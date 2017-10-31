// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef AudioWorklet_h
#define AudioWorklet_h

#include "core/workers/Worklet.h"
#include "modules/ModulesExport.h"
#include "platform/heap/Handle.h"

namespace blink {

class LocalFrame;

class MODULES_EXPORT AudioWorklet final : public Worklet {
  WTF_MAKE_NONCOPYABLE(AudioWorklet);

 public:
  static AudioWorklet* Create(LocalFrame*);
  ~AudioWorklet() override;

  DECLARE_VIRTUAL_TRACE();

 private:
  explicit AudioWorklet(LocalFrame*);

  // Implements Worklet.
  bool NeedsToCreateGlobalScope() final;
  WorkletGlobalScopeProxy* CreateGlobalScope() final;
};

}  // namespace blink

#endif  // AudioWorklet_h
