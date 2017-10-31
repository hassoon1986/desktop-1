// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py.
// DO NOT MODIFY!

// This file has been generated from the Jinja2 template in
// third_party/WebKit/Source/bindings/templates/interface.h.tmpl

// clang-format off
#ifndef V8TestInterfaceEventTarget_h
#define V8TestInterfaceEventTarget_h

#include "bindings/core/v8/GeneratedCodeHelper.h"
#include "bindings/core/v8/NativeValueTraits.h"
#include "bindings/core/v8/ToV8ForCore.h"
#include "bindings/core/v8/V8BindingForCore.h"
#include "bindings/core/v8/V8EventTarget.h"
#include "bindings/tests/idls/core/TestInterfaceEventTarget.h"
#include "core/CoreExport.h"
#include "platform/bindings/ScriptWrappable.h"
#include "platform/bindings/V8DOMWrapper.h"
#include "platform/bindings/WrapperTypeInfo.h"
#include "platform/heap/Handle.h"

namespace blink {

class V8TestInterfaceEventTargetConstructor {
  STATIC_ONLY(V8TestInterfaceEventTargetConstructor);
 public:
  static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate*, const DOMWrapperWorld&);
  CORE_EXPORT static void NamedConstructorAttributeGetter(v8::Local<v8::Name> propertyName, const v8::PropertyCallbackInfo<v8::Value>& info);
  CORE_EXPORT static const WrapperTypeInfo wrapperTypeInfo;
};

class V8TestInterfaceEventTarget {
  STATIC_ONLY(V8TestInterfaceEventTarget);
 public:
  CORE_EXPORT static bool hasInstance(v8::Local<v8::Value>, v8::Isolate*);
  static v8::Local<v8::Object> findInstanceInPrototypeChain(v8::Local<v8::Value>, v8::Isolate*);
  CORE_EXPORT static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate*, const DOMWrapperWorld&);
  static TestInterfaceEventTarget* toImpl(v8::Local<v8::Object> object) {
    return ToScriptWrappable(object)->ToImpl<TestInterfaceEventTarget>();
  }
  CORE_EXPORT static TestInterfaceEventTarget* toImplWithTypeCheck(v8::Isolate*, v8::Local<v8::Value>);
  CORE_EXPORT static const WrapperTypeInfo wrapperTypeInfo;
  static void Trace(Visitor* visitor, ScriptWrappable* scriptWrappable) {
    visitor->Trace(scriptWrappable->ToImpl<TestInterfaceEventTarget>());
  }
  static void TraceWrappers(WrapperVisitor* visitor, ScriptWrappable* scriptWrappable) {
    visitor->TraceWrappersWithManualWriteBarrier(scriptWrappable->ToImpl<TestInterfaceEventTarget>());
  }
  static const int eventListenerCacheIndex = kV8DefaultWrapperInternalFieldCount + 0;
  static const int internalFieldCount = kV8DefaultWrapperInternalFieldCount + 1;

  // Callback functions

  static void InstallRuntimeEnabledFeaturesOnTemplate(
      v8::Isolate*,
      const DOMWrapperWorld&,
      v8::Local<v8::FunctionTemplate> interface_template);
};

template <>
struct NativeValueTraits<TestInterfaceEventTarget> : public NativeValueTraitsBase<TestInterfaceEventTarget> {
  CORE_EXPORT static TestInterfaceEventTarget* NativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
};

template <>
struct V8TypeOf<TestInterfaceEventTarget> {
  typedef V8TestInterfaceEventTarget Type;
};

}  // namespace blink

#endif  // V8TestInterfaceEventTarget_h
