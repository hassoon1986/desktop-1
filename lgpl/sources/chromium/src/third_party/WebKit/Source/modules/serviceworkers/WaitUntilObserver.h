// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WaitUntilObserver_h
#define WaitUntilObserver_h

#include "modules/ModulesExport.h"
#include "modules/serviceworkers/ServiceWorkerGlobalScopeClient.h"
#include "platform/Timer.h"
#include "platform/wtf/Forward.h"
#include "platform/wtf/Functional.h"

namespace blink {

class ExceptionState;
class ExecutionContext;
class ScriptPromise;
class ScriptState;

// Created for each ExtendableEvent instance.
class MODULES_EXPORT WaitUntilObserver final
    : public GarbageCollectedFinalized<WaitUntilObserver> {
 public:
  using PromiseSettledCallback = Function<void(const ScriptValue&)>;

  enum EventType {
    kActivate,
    kCanMakePayment,
    kFetch,
    kInstall,
    kMessage,
    kNotificationClick,
    kNotificationClose,
    kPaymentRequest,
    kPush,
    kSync,
    kBackgroundFetchAbort,
    kBackgroundFetchClick,
    kBackgroundFetchFail,
    kBackgroundFetched
  };

  static WaitUntilObserver* Create(ExecutionContext*, EventType, int event_id);

  // Must be called before dispatching the event.
  void WillDispatchEvent();
  // Must be called after dispatching the event. If |event_dispatch_failed| is
  // true, then DidDispatchEvent() immediately reports to
  // ServiceWorkerGlobalScopeClient that the event finished, without waiting for
  // all waitUntil promises to settle.
  void DidDispatchEvent(bool event_dispatch_failed);

  // Observes the promise and delays reporting to ServiceWorkerGlobalScopeClient
  // that the event completed until the given promise is resolved or rejected.
  // WaitUntil may be called multiple times. The event is extended until all
  // promises have settled.
  // If provided, |on_promise_fulfilled| or |on_promise_rejected| is invoked
  // once |script_promise| fulfills or rejects. This enables the caller to do
  // custom handling.
  void WaitUntil(
      ScriptState*,
      ScriptPromise /* script_promise */,
      ExceptionState&,
      std::unique_ptr<PromiseSettledCallback> on_promise_fulfilled = nullptr,
      std::unique_ptr<PromiseSettledCallback> on_promise_rejected = nullptr);

  DECLARE_VIRTUAL_TRACE();

 private:
  friend class InternalsServiceWorker;
  class ThenFunction;

  enum class EventDispatchState {
    // Event dispatch has not yet started.
    kInitial,
    // Event dispatch has started but not yet finished.
    kDispatching,
    // Event dispatch completed. There may still be outstanding waitUntil
    // promises that must settle before notifying ServiceWorkerGlobalScopeClient
    // that the event finished.
    kDispatched,
    // Event dispatch failed. Any outstanding waitUntil promises are ignored.
    kFailed
  };

  WaitUntilObserver(ExecutionContext*, EventType, int event_id);

  void IncrementPendingPromiseCount();
  void DecrementPendingPromiseCount();

  // Enqueued as a microtask when a promise passed to a waitUntil() call that is
  // associated with this observer was fulfilled.
  void OnPromiseFulfilled();
  // Enqueued as a microtask when a promise passed to a waitUntil() call that is
  // associated with this observer was rejected.
  void OnPromiseRejected();

  void ConsumeWindowInteraction(TimerBase*);

  void MaybeCompleteEvent();

  Member<ExecutionContext> execution_context_;
  EventType type_;
  int event_id_;
  int pending_promises_ = 0;
  EventDispatchState event_dispatch_state_ = EventDispatchState::kInitial;
  bool has_rejected_promise_ = false;
  double event_dispatch_time_ = 0;
  TaskRunnerTimer<WaitUntilObserver> consume_window_interaction_timer_;
};

}  // namespace blink

#endif  // WaitUntilObserver_h
