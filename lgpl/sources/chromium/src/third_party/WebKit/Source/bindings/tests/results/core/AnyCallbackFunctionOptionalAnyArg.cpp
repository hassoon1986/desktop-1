// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py.
// DO NOT MODIFY!

// This file has been generated from the Jinja2 template in
// third_party/WebKit/Source/bindings/templates/callback_function.cpp.tmpl

// clang-format off

#include "AnyCallbackFunctionOptionalAnyArg.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/NativeValueTraitsImpl.h"
#include "bindings/core/v8/ScriptValue.h"
#include "bindings/core/v8/ToV8ForCore.h"
#include "bindings/core/v8/V8BindingForCore.h"
#include "core/dom/ExecutionContext.h"
#include "platform/bindings/ScriptState.h"
#include "platform/wtf/Assertions.h"

namespace blink {

// static
AnyCallbackFunctionOptionalAnyArg* AnyCallbackFunctionOptionalAnyArg::Create(ScriptState* scriptState, v8::Local<v8::Value> callback) {
  if (IsUndefinedOrNull(callback))
    return nullptr;
  return new AnyCallbackFunctionOptionalAnyArg(scriptState, v8::Local<v8::Function>::Cast(callback));
}

AnyCallbackFunctionOptionalAnyArg::AnyCallbackFunctionOptionalAnyArg(ScriptState* scriptState, v8::Local<v8::Function> callback)
    : script_state_(scriptState),
    callback_(scriptState->GetIsolate(), this, callback) {
  DCHECK(!callback_.IsEmpty());
}

DEFINE_TRACE_WRAPPERS(AnyCallbackFunctionOptionalAnyArg) {
  visitor->TraceWrappers(callback_.Cast<v8::Value>());
}

bool AnyCallbackFunctionOptionalAnyArg::call(ScriptWrappable* scriptWrappable, ScriptValue optionalAnyArg, ScriptValue& returnValue) {
  if (callback_.IsEmpty())
    return false;

  if (!script_state_->ContextIsValid())
    return false;

  // TODO(bashi): Make sure that using DummyExceptionStateForTesting is OK.
  // crbug.com/653769
  DummyExceptionStateForTesting exceptionState;

  ExecutionContext* context = ExecutionContext::From(script_state_.Get());
  DCHECK(context);
  if (context->IsContextSuspended() || context->IsContextDestroyed())
    return false;

  ScriptState::Scope scope(script_state_.Get());
  v8::Isolate* isolate = script_state_->GetIsolate();

  v8::Local<v8::Value> thisValue = ToV8(
      scriptWrappable,
      script_state_->GetContext()->Global(),
      isolate);

  v8::Local<v8::Value> v8_optionalAnyArg = optionalAnyArg.V8Value();
  v8::Local<v8::Value> argv[] = { v8_optionalAnyArg };
  v8::TryCatch exceptionCatcher(isolate);
  exceptionCatcher.SetVerbose(true);

  v8::Local<v8::Value> v8ReturnValue;
  if (!V8ScriptRunner::CallFunction(callback_.NewLocal(isolate),
                                    context,
                                    thisValue,
                                    1,
                                    argv,
                                    isolate).ToLocal(&v8ReturnValue)) {
    return false;
  }

  ScriptValue cppValue = ScriptValue(ScriptState::Current(script_state_->GetIsolate()), v8ReturnValue);
  returnValue = cppValue;
  return true;
}

AnyCallbackFunctionOptionalAnyArg* NativeValueTraits<AnyCallbackFunctionOptionalAnyArg>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
  AnyCallbackFunctionOptionalAnyArg* nativeValue = AnyCallbackFunctionOptionalAnyArg::Create(ScriptState::Current(isolate), value);
  if (!nativeValue) {
    exceptionState.ThrowTypeError(ExceptionMessages::FailedToConvertJSValue(
        "AnyCallbackFunctionOptionalAnyArg"));
  }
  return nativeValue;
}

}  // namespace blink
