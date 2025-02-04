/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "public/web/WebSecurityPolicy.h"

#include "core/loader/FrameLoader.h"
#include "platform/weborigin/SchemeRegistry.h"
#include "platform/weborigin/SecurityOrigin.h"
#include "platform/weborigin/SecurityPolicy.h"
#include "public/platform/WebSecurityOrigin.h"
#include "public/platform/WebString.h"
#include "public/platform/WebURL.h"

namespace blink {

void WebSecurityPolicy::RegisterURLSchemeAsDisplayIsolated(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsDisplayIsolated(scheme);
}

void WebSecurityPolicy::RegisterURLSchemeAsAllowingServiceWorkers(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsAllowingServiceWorkers(scheme);
}

void WebSecurityPolicy::RegisterURLSchemeAsAllowingWasmEvalCSP(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsAllowingWasmEvalCSP(scheme);
}

void WebSecurityPolicy::RegisterURLSchemeAsSupportingFetchAPI(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsSupportingFetchAPI(scheme);
}

void WebSecurityPolicy::RegisterURLSchemeAsFirstPartyWhenTopLevel(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsFirstPartyWhenTopLevel(scheme);
}

#if defined(OPERA_DESKTOP)
void WebSecurityPolicy::AddOriginAsFirstPartyForSubframes(
    const WebSecurityOrigin& origin) {
  scoped_refptr<const SecurityOrigin> o = origin;
  SecurityPolicy::AddOriginAsFirstPartyForSubframes(o->IsolatedCopy());
}
#endif  // OPERA_DESKTOP

void WebSecurityPolicy::AddOriginAccessWhitelistEntry(
    const WebURL& source_origin,
    const WebString& destination_protocol,
    const WebString& destination_host,
    bool allow_destination_subdomains) {
  SecurityPolicy::AddOriginAccessWhitelistEntry(
      *SecurityOrigin::Create(source_origin), destination_protocol,
      destination_host, allow_destination_subdomains);
}

void WebSecurityPolicy::RemoveOriginAccessWhitelistEntry(
    const WebURL& source_origin,
    const WebString& destination_protocol,
    const WebString& destination_host,
    bool allow_destination_subdomains) {
  SecurityPolicy::RemoveOriginAccessWhitelistEntry(
      *SecurityOrigin::Create(source_origin), destination_protocol,
      destination_host, allow_destination_subdomains);
}

void WebSecurityPolicy::ResetOriginAccessWhitelists() {
  SecurityPolicy::ResetOriginAccessWhitelists();
}

void WebSecurityPolicy::AddOriginTrustworthyWhiteList(
    const WebSecurityOrigin& origin) {
  SecurityPolicy::AddOriginTrustworthyWhiteList(*origin.Get());
}

void WebSecurityPolicy::AddSchemeToBypassSecureContextWhitelist(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeBypassingSecureContextCheck(scheme);
}

WebString WebSecurityPolicy::GenerateReferrerHeader(
    WebReferrerPolicy referrer_policy,
    const WebURL& url,
    const WebString& referrer) {
  return SecurityPolicy::GenerateReferrer(
             static_cast<ReferrerPolicy>(referrer_policy), url, referrer)
      .referrer;
}

void WebSecurityPolicy::RegisterURLSchemeAsNotAllowingJavascriptURLs(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsNotAllowingJavascriptURLs(scheme);
}

void WebSecurityPolicy::RegisterURLSchemeAsAllowedForReferrer(
    const WebString& scheme) {
  SchemeRegistry::RegisterURLSchemeAsAllowedForReferrer(scheme);
}

}  // namespace blink
