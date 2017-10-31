// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py.
// DO NOT MODIFY!

// This file has been generated from the Jinja2 template in
// third_party/WebKit/Source/bindings/templates/interface.h.tmpl

// clang-format off
#ifndef V8TestInterfaceNamedConstructor2_h
#define V8TestInterfaceNamedConstructor2_h

#include "bindings/core/v8/GeneratedCodeHelper.h"
#include "bindings/core/v8/NativeValueTraits.h"
#include "bindings/core/v8/ToV8ForCore.h"
#include "bindings/core/v8/V8BindingForCore.h"
#include "bindings/tests/idls/core/TestInterfaceNamedConstructor2.h"
#include "core/CoreExport.h"
#include "platform/bindings/ScriptWrappable.h"
#include "platform/bindings/V8DOMWrapper.h"
#include "platform/bindings/WrapperTypeInfo.h"
#include "platform/heap/Handle.h"

namespace blink {

class V8TestInterfaceNamedConstructor2Constructor {
  STATIC_ONLY(V8TestInterfaceNamedConstructor2Constructor);
 public:
  static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate*, const DOMWrapperWorld&);
  CORE_EXPORT static void NamedConstructorAttributeGetter(v8::Local<v8::Name> propertyName, const v8::PropertyCallbackInfo<v8::Value>& info);
  CORE_EXPORT static const WrapperTypeInfo wrapperTypeInfo;
};

class V8TestInterfaceNamedConstructor2 {
  STATIC_ONLY(V8TestInterfaceNamedConstructor2);
 public:
  CORE_EXPORT static bool hasInstance(v8::Local<v8::Value>, v8::Isolate*);
  static v8::Local<v8::Object> findInstanceInPrototypeChain(v8::Local<v8::Value>, v8::Isolate*);
  CORE_EXPORT static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate*, const DOMWrapperWorld&);
  static TestInterfaceNamedConstructor2* toImpl(v8::Local<v8::Object> object) {
    return ToScriptWrappable(object)->ToImpl<TestInterfaceNamedConstructor2>();
  }
  CORE_EXPORT static TestInterfaceNamedConstructor2* toImplWithTypeCheck(v8::Isolate*, v8::Local<v8::Value>);
  CORE_EXPORT static const WrapperTypeInfo wrapperTypeInfo;
  static void Trace(Visitor* visitor, ScriptWrappable* scriptWrappable) {
    visitor->Trace(scriptWrappable->ToImpl<TestInterfaceNamedConstructor2>());
  }
  static void TraceWrappers(WrapperVisitor* visitor, ScriptWrappable* scriptWrappable) {
    visitor->TraceWrappersWithManualWriteBarrier(scriptWrappable->ToImpl<TestInterfaceNamedConstructor2>());
  }
  static const int internalFieldCount = kV8DefaultWrapperInternalFieldCount + 0;

  // Callback functions

  static void InstallRuntimeEnabledFeaturesOnTemplate(
      v8::Isolate*,
      const DOMWrapperWorld&,
      v8::Local<v8::FunctionTemplate> interface_template);
};

template <>
struct NativeValueTraits<TestInterfaceNamedConstructor2> : public NativeValueTraitsBase<TestInterfaceNamedConstructor2> {
  CORE_EXPORT static TestInterfaceNamedConstructor2* NativeValue(v8::Isolate*, v8::Local<v8::Value>, ExceptionState&);
};

template <>
struct V8TypeOf<TestInterfaceNamedConstructor2> {
  typedef V8TestInterfaceNamedConstructor2 Type;
};

}  // namespace blink

#endif  // V8TestInterfaceNamedConstructor2_h
