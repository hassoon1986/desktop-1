// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py.
// DO NOT MODIFY!

// This file has been generated from the Jinja2 template in
// third_party/WebKit/Source/bindings/templates/union_container.cpp.tmpl

// clang-format off
#include "NestedUnionType.h"

#include "bindings/core/v8/ByteStringOrNodeList.h"
#include "bindings/core/v8/IDLTypes.h"
#include "bindings/core/v8/NativeValueTraitsImpl.h"
#include "bindings/core/v8/ToV8ForCore.h"
#include "bindings/core/v8/V8Event.h"
#include "bindings/core/v8/V8Node.h"
#include "bindings/core/v8/V8NodeList.h"
#include "bindings/core/v8/V8XMLHttpRequest.h"
#include "core/dom/NameNodeList.h"
#include "core/dom/NodeList.h"
#include "core/dom/StaticNodeList.h"
#include "core/html/LabelsNodeList.h"

namespace blink {

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord() : m_type(SpecificTypeNone) {}

Event* NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsEvent() const {
  DCHECK(isEvent());
  return m_event;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setEvent(Event* value) {
  DCHECK(isNull());
  m_event = value;
  m_type = SpecificTypeEvent;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromEvent(Event* value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setEvent(value);
  return container;
}

const Vector<int32_t>& NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsLongSequence() const {
  DCHECK(isLongSequence());
  return m_longSequence;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setLongSequence(const Vector<int32_t>& value) {
  DCHECK(isNull());
  m_longSequence = value;
  m_type = SpecificTypeLongSequence;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromLongSequence(const Vector<int32_t>& value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setLongSequence(value);
  return container;
}

Node* NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsNode() const {
  DCHECK(isNode());
  return m_node;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setNode(Node* value) {
  DCHECK(isNull());
  m_node = value;
  m_type = SpecificTypeNode;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromNode(Node* value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setNode(value);
  return container;
}

const String& NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsString() const {
  DCHECK(isString());
  return m_string;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setString(const String& value) {
  DCHECK(isNull());
  m_string = value;
  m_type = SpecificTypeString;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromString(const String& value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setString(value);
  return container;
}

const HeapVector<std::pair<String, ByteStringOrNodeList>>& NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsStringByteStringOrNodeListRecord() const {
  DCHECK(isStringByteStringOrNodeListRecord());
  return m_stringByteStringOrNodeListRecord;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setStringByteStringOrNodeListRecord(const HeapVector<std::pair<String, ByteStringOrNodeList>>& value) {
  DCHECK(isNull());
  m_stringByteStringOrNodeListRecord = value;
  m_type = SpecificTypeStringByteStringOrNodeListRecord;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromStringByteStringOrNodeListRecord(const HeapVector<std::pair<String, ByteStringOrNodeList>>& value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setStringByteStringOrNodeListRecord(value);
  return container;
}

XMLHttpRequest* NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::getAsXMLHttpRequest() const {
  DCHECK(isXMLHttpRequest());
  return m_xmlHttpRequest;
}

void NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::setXMLHttpRequest(XMLHttpRequest* value) {
  DCHECK(isNull());
  m_xmlHttpRequest = value;
  m_type = SpecificTypeXMLHttpRequest;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::fromXMLHttpRequest(XMLHttpRequest* value) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord container;
  container.setXMLHttpRequest(value);
  return container;
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord(const NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord&) = default;
NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::~NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord() = default;
NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord& NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::operator=(const NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord&) = default;

DEFINE_TRACE(NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord) {
  visitor->Trace(m_event);
  visitor->Trace(m_node);
  visitor->Trace(m_stringByteStringOrNodeListRecord);
  visitor->Trace(m_xmlHttpRequest);
}

void V8NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::toImpl(v8::Isolate* isolate, v8::Local<v8::Value> v8Value, NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord& impl, UnionTypeConversionMode conversionMode, ExceptionState& exceptionState) {
  if (v8Value.IsEmpty())
    return;

  if (conversionMode == UnionTypeConversionMode::kNullable && IsUndefinedOrNull(v8Value))
    return;

  if (V8Event::hasInstance(v8Value, isolate)) {
    Event* cppValue = V8Event::toImpl(v8::Local<v8::Object>::Cast(v8Value));
    impl.setEvent(cppValue);
    return;
  }

  if (V8Node::hasInstance(v8Value, isolate)) {
    Node* cppValue = V8Node::toImpl(v8::Local<v8::Object>::Cast(v8Value));
    impl.setNode(cppValue);
    return;
  }

  if (V8XMLHttpRequest::hasInstance(v8Value, isolate)) {
    XMLHttpRequest* cppValue = V8XMLHttpRequest::toImpl(v8::Local<v8::Object>::Cast(v8Value));
    impl.setXMLHttpRequest(cppValue);
    return;
  }

  if (HasCallableIteratorSymbol(isolate, v8Value, exceptionState)) {
    Vector<int32_t> cppValue = NativeValueTraits<IDLSequence<IDLLong>>::NativeValue(isolate, v8Value, exceptionState);
    if (exceptionState.HadException())
      return;
    impl.setLongSequence(cppValue);
    return;
  }

  if (v8Value->IsObject()) {
    HeapVector<std::pair<String, ByteStringOrNodeList>> cppValue = NativeValueTraits<IDLRecord<IDLString, ByteStringOrNodeList>>::NativeValue(isolate, v8Value, exceptionState);
    if (exceptionState.HadException())
      return;
    impl.setStringByteStringOrNodeListRecord(cppValue);
    return;
  }

  {
    V8StringResource<> cppValue = v8Value;
    if (!cppValue.Prepare(exceptionState))
      return;
    impl.setString(cppValue);
    return;
  }
}

v8::Local<v8::Value> ToV8(const NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord& impl, v8::Local<v8::Object> creationContext, v8::Isolate* isolate) {
  switch (impl.m_type) {
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeNone:
      return v8::Null(isolate);
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeEvent:
      return ToV8(impl.getAsEvent(), creationContext, isolate);
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeLongSequence:
      return ToV8(impl.getAsLongSequence(), creationContext, isolate);
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeNode:
      return ToV8(impl.getAsNode(), creationContext, isolate);
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeString:
      return V8String(isolate, impl.getAsString());
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeStringByteStringOrNodeListRecord:
      return ToV8(impl.getAsStringByteStringOrNodeListRecord(), creationContext, isolate);
    case NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::SpecificTypeXMLHttpRequest:
      return ToV8(impl.getAsXMLHttpRequest(), creationContext, isolate);
    default:
      NOTREACHED();
  }
  return v8::Local<v8::Value>();
}

NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord NativeValueTraits<NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord>::NativeValue(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
  NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord impl;
  V8NodeOrLongSequenceOrEventOrXMLHttpRequestOrStringOrStringByteStringOrNodeListRecord::toImpl(isolate, value, impl, UnionTypeConversionMode::kNotNullable, exceptionState);
  return impl;
}

}  // namespace blink
