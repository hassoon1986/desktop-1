// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform/DataResourceHelper.h"

#include "public/platform/Platform.h"
#include "public/platform/WebData.h"

namespace blink {

String GetDataResourceAsASCIIString(const char* resource) {
  const WebData& resource_data = Platform::Current()->GetDataResource(resource);
  String data_string(resource_data.Data(), resource_data.size());
  DCHECK(!data_string.IsEmpty());
  DCHECK(data_string.ContainsOnlyASCII());
  return data_string;
}

}  // namespace blink
