/*
 * Copyright (C) 2011 Google Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IdentifiersFactory_h
#define IdentifiersFactory_h

#include "base/unguessable_token.h"
#include "core/CoreExport.h"
#include "platform/wtf/Allocator.h"
#include "platform/wtf/text/WTFString.h"

namespace blink {

class DocumentLoader;
class Frame;
class LocalFrame;
class InspectedFrames;

class CORE_EXPORT IdentifiersFactory {
  STATIC_ONLY(IdentifiersFactory);

 public:
  static String CreateIdentifier();

  static String RequestId(DocumentLoader*, unsigned long identifier);
  // Same as above, but can only be used on reuquests that are guaranteed
  // to be subresources, not main resource.
  static String SubresourceRequestId(unsigned long identifier);

  // Returns embedder-provided frame token that is consistent across processes
  // and can be used for request / call attribution to the context frame.
  static String FrameId(Frame*);
  static LocalFrame* FrameById(InspectedFrames*, const String&);

  static String LoaderId(DocumentLoader*);

  static String IdFromToken(const base::UnguessableToken&);

 private:
  static String AddProcessIdPrefixTo(int id);
  static int RemoveProcessIdPrefixFrom(const String&, bool* ok);
};

}  // namespace blink

#endif  // IdentifiersFactory_h
