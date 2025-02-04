// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "modules/webaudio/AudioWorkletThread.h"

#include <memory>
#include "bindings/core/v8/ScriptModule.h"
#include "bindings/core/v8/ScriptSourceCode.h"
#include "bindings/core/v8/SourceLocation.h"
#include "bindings/core/v8/V8CacheOptions.h"
#include "bindings/core/v8/V8GCController.h"
#include "bindings/core/v8/WorkerOrWorkletScriptController.h"
#include "core/inspector/ConsoleMessage.h"
#include "core/origin_trials/OriginTrialContext.h"
#include "core/testing/PageTestBase.h"
#include "core/workers/GlobalScopeCreationParams.h"
#include "core/workers/WorkerBackingThread.h"
#include "core/workers/WorkerInspectorProxy.h"
#include "core/workers/WorkerOrWorkletGlobalScope.h"
#include "core/workers/WorkerReportingProxy.h"
#include "platform/CrossThreadFunctional.h"
#include "platform/WaitableEvent.h"
#include "platform/WebThreadSupportingGC.h"
#include "platform/heap/Handle.h"
#include "platform/loader/fetch/AccessControlStatus.h"
#include "platform/loader/fetch/ResourceLoaderOptions.h"
#include "platform/testing/TestingPlatformSupport.h"
#include "platform/testing/UnitTestHelpers.h"
#include "platform/wtf/text/TextPosition.h"
#include "public/platform/Platform.h"
#include "public/platform/WebURLRequest.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace blink {

class AudioWorkletThreadTest : public PageTestBase {
 public:
  void SetUp() override {
    AudioWorkletThread::CreateSharedBackingThreadForTest();
    PageTestBase::SetUp(IntSize());
    Document* document = &GetDocument();
    document->SetURL(KURL("https://example.com/"));
    document->UpdateSecurityOrigin(SecurityOrigin::Create(document->Url()));
    reporting_proxy_ = std::make_unique<WorkerReportingProxy>();
  }

  std::unique_ptr<AudioWorkletThread> CreateAudioWorkletThread() {
    std::unique_ptr<AudioWorkletThread> thread =
        AudioWorkletThread::Create(nullptr, *reporting_proxy_);
    Document* document = &GetDocument();
    thread->Start(
        std::make_unique<GlobalScopeCreationParams>(
            document->Url(), document->UserAgent(document->Url()),
            nullptr /* content_security_policy_parsed_headers */,
            document->GetReferrerPolicy(), document->GetSecurityOrigin(),
            document->IsSecureContext(), nullptr /* worker_clients */,
            document->AddressSpace(),
            OriginTrialContext::GetTokens(document).get(),
            base::UnguessableToken::Create(), nullptr /* worker_settings */,
            kV8CacheOptionsDefault),
        WTF::nullopt, WorkerInspectorProxy::PauseOnWorkerStart::kDontPause,
        ParentFrameTaskRunners::Create());
    return thread;
  }

  // Attempts to run some simple script for |thread|.
  void CheckWorkletCanExecuteScript(WorkerThread* thread) {
    WaitableEvent wait_event;
    thread->GetWorkerBackingThread().BackingThread().PostTask(
        FROM_HERE,
        CrossThreadBind(&AudioWorkletThreadTest::ExecuteScriptInWorklet,
                        CrossThreadUnretained(this),
                        CrossThreadUnretained(thread),
                        CrossThreadUnretained(&wait_event)));
    wait_event.Wait();
  }

 private:
  void ExecuteScriptInWorklet(WorkerThread* thread, WaitableEvent* wait_event) {
    ScriptState* script_state =
        thread->GlobalScope()->ScriptController()->GetScriptState();
    EXPECT_TRUE(script_state);
    ScriptState::Scope scope(script_state);
    KURL js_url("https://example.com/worklet.js");
    ScriptModule module = ScriptModule::Compile(
        script_state->GetIsolate(), "var counter = 0; ++counter;", js_url,
        js_url, ScriptFetchOptions(), kSharableCrossOrigin,
        TextPosition::MinimumPosition(), ASSERT_NO_EXCEPTION);
    EXPECT_FALSE(module.IsNull());
    ScriptValue exception = module.Instantiate(script_state);
    EXPECT_TRUE(exception.IsEmpty());
    ScriptValue value = module.Evaluate(script_state);
    EXPECT_TRUE(value.IsEmpty());
    wait_event->Signal();
  }

  std::unique_ptr<WorkerReportingProxy> reporting_proxy_;
};

TEST_F(AudioWorkletThreadTest, Basic) {
  std::unique_ptr<AudioWorkletThread> worklet = CreateAudioWorkletThread();
  CheckWorkletCanExecuteScript(worklet.get());
  worklet->Terminate();
  worklet->WaitForShutdownForTesting();
}

// Tests that the same WebThread is used for new worklets if the WebThread is
// still alive.
TEST_F(AudioWorkletThreadTest, CreateSecondAndTerminateFirst) {
  // Create the first worklet and wait until it is initialized.
  std::unique_ptr<AudioWorkletThread> first_worklet =
      CreateAudioWorkletThread();
  WebThreadSupportingGC* first_thread =
      &first_worklet->GetWorkerBackingThread().BackingThread();
  CheckWorkletCanExecuteScript(first_worklet.get());
  v8::Isolate* first_isolate = first_worklet->GetIsolate();
  ASSERT_TRUE(first_isolate);

  // Create the second worklet and immediately destroy the first worklet.
  std::unique_ptr<AudioWorkletThread> second_worklet =
      CreateAudioWorkletThread();
  // We don't use terminateAndWait here to avoid forcible termination.
  first_worklet->Terminate();
  first_worklet->WaitForShutdownForTesting();

  // Wait until the second worklet is initialized. Verify that the second
  // worklet is using the same thread and Isolate as the first worklet.
  WebThreadSupportingGC* second_thread =
      &second_worklet->GetWorkerBackingThread().BackingThread();
  ASSERT_EQ(first_thread, second_thread);

  v8::Isolate* second_isolate = second_worklet->GetIsolate();
  ASSERT_TRUE(second_isolate);
  EXPECT_EQ(first_isolate, second_isolate);

  // Verify that the worklet can still successfully execute script.
  CheckWorkletCanExecuteScript(second_worklet.get());

  second_worklet->Terminate();
  second_worklet->WaitForShutdownForTesting();
}

// Tests that a new WebThread is created if all existing worklets are
// terminated before a new worklet is created.
TEST_F(AudioWorkletThreadTest, TerminateFirstAndCreateSecond) {
  // Create the first worklet, wait until it is initialized, and terminate it.
  std::unique_ptr<AudioWorkletThread> worklet = CreateAudioWorkletThread();
  WebThreadSupportingGC* first_thread =
      &worklet->GetWorkerBackingThread().BackingThread();
  CheckWorkletCanExecuteScript(worklet.get());

  // We don't use terminateAndWait here to avoid forcible termination.
  worklet->Terminate();
  worklet->WaitForShutdownForTesting();

  // Create the second worklet. The backing thread is same.
  worklet = CreateAudioWorkletThread();
  WebThreadSupportingGC* second_thread =
      &worklet->GetWorkerBackingThread().BackingThread();
  EXPECT_EQ(first_thread, second_thread);
  CheckWorkletCanExecuteScript(worklet.get());

  worklet->Terminate();
  worklet->WaitForShutdownForTesting();
}

// Tests that v8::Isolate and WebThread are correctly set-up if a worklet is
// created while another is terminating.
TEST_F(AudioWorkletThreadTest, CreatingSecondDuringTerminationOfFirst) {
  std::unique_ptr<AudioWorkletThread> first_worklet =
      CreateAudioWorkletThread();
  CheckWorkletCanExecuteScript(first_worklet.get());
  v8::Isolate* first_isolate = first_worklet->GetIsolate();
  ASSERT_TRUE(first_isolate);

  // Request termination of the first worklet and create the second worklet
  // as soon as possible. We don't wait for its termination.
  // Note: We rely on the assumption that the termination steps don't run
  // on the worklet thread so quickly. This could be a source of flakiness.
  first_worklet->Terminate();
  std::unique_ptr<AudioWorkletThread> second_worklet =
      CreateAudioWorkletThread();

  v8::Isolate* second_isolate = second_worklet->GetIsolate();
  ASSERT_TRUE(second_isolate);
  EXPECT_EQ(first_isolate, second_isolate);

  // Verify that the isolate can run some scripts correctly in the second
  // worklet.
  CheckWorkletCanExecuteScript(second_worklet.get());
  second_worklet->Terminate();
  second_worklet->WaitForShutdownForTesting();
}

}  // namespace blink
