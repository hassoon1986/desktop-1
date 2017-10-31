// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PtrUtil_h
#define PtrUtil_h

#include "base/memory/ptr_util.h"
#include "platform/wtf/TypeTraits.h"

#include <memory>
#include <type_traits>

namespace WTF {

template <typename T>
std::unique_ptr<T> WrapUnique(T* ptr) {
  static_assert(
      !WTF::IsGarbageCollectedType<T>::value,
      "Garbage collected types should not be stored in std::unique_ptr!");
  return std::unique_ptr<T>(ptr);
}

template <typename T>
std::unique_ptr<T[]> WrapArrayUnique(T* ptr) {
  static_assert(
      !WTF::IsGarbageCollectedType<T>::value,
      "Garbage collected types should not be stored in std::unique_ptr!");
  return std::unique_ptr<T[]>(ptr);
}

// WTF::makeUnique is base::MakeUnique. See base/ptr_util.h for documentation.
template <typename T, typename... Args>
auto MakeUnique(Args&&... args)
    -> decltype(base::MakeUnique<T>(std::forward<Args>(args)...)) {
  static_assert(
      !WTF::IsGarbageCollectedType<typename std::remove_extent<T>::type>::value,
      "Garbage collected types should not be stored in std::unique_ptr!");
  return base::MakeUnique<T>(std::forward<Args>(args)...);
}

}  // namespace WTF

using WTF::WrapArrayUnique;

#endif  // PtrUtil_h
