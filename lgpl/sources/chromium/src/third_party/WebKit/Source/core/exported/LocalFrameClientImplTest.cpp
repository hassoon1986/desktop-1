/*
 * Copyright (C) 2011, 2012 Google Inc. All rights reserved.
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

#include "core/exported/LocalFrameClientImpl.h"

#include "core/frame/FrameTestHelpers.h"
#include "core/frame/WebLocalFrameImpl.h"
#include "platform/weborigin/KURL.h"
#include "platform/wtf/text/CString.h"
#include "platform/wtf/text/WTFString.h"
#include "public/web/WebFrameClient.h"
#include "public/web/WebSettings.h"
#include "public/web/WebView.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;

namespace blink {
namespace {

class LocalFrameMockWebFrameClient
    : public FrameTestHelpers::TestWebFrameClient {
 public:
  ~LocalFrameMockWebFrameClient() override = default;

  MOCK_METHOD1(UserAgentOverride, WebString(const WebURL&));
};

class LocalFrameClientImplTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ON_CALL(web_frame_client_, UserAgentOverride(_))
        .WillByDefault(Return(WebString()));

    helper_.Initialize(&web_frame_client_);
    // FIXME: http://crbug.com/363843. This needs to find a better way to
    // not create graphics layers.
    helper_.GetWebView()->GetSettings()->SetAcceleratedCompositingEnabled(
        false);
  }

  void TearDown() override {
    // Tearing down the WebView by resetting the helper will call
    // UserAgentOverride() in order to store the information for detached
    // requests.
    EXPECT_CALL(WebFrameClient(), UserAgentOverride(_));
    helper_.Reset();
  }

  WebString UserAgent() {
    // The test always returns the same user agent, regardless of the URL passed
    // in.
    KURL dummy_url("about:blank");
    WTF::CString user_agent = GetLocalFrameClient().UserAgent(dummy_url).Utf8();
    return WebString::FromUTF8(user_agent.data(), user_agent.length());
  }

  WebLocalFrameImpl* MainFrame() { return helper_.LocalMainFrame(); }
  Document& GetDocument() { return *MainFrame()->GetFrame()->GetDocument(); }
  LocalFrameMockWebFrameClient& WebFrameClient() { return web_frame_client_; }
  LocalFrameClient& GetLocalFrameClient() {
    return *ToLocalFrameClientImpl(MainFrame()->GetFrame()->Client());
  }

 private:
  LocalFrameMockWebFrameClient web_frame_client_;
  FrameTestHelpers::WebViewHelper helper_;
};

TEST_F(LocalFrameClientImplTest, UserAgentOverride) {
  const WebString default_user_agent = UserAgent();
  const WebString override_user_agent = WebString::FromUTF8("dummy override");

  // Override the user agent and make sure we get it back.
  EXPECT_CALL(WebFrameClient(), UserAgentOverride(_))
      .WillOnce(Return(override_user_agent));
  EXPECT_TRUE(override_user_agent.Equals(UserAgent()));
  Mock::VerifyAndClearExpectations(&WebFrameClient());

  // Remove the override and make sure we get the original back.
  EXPECT_CALL(WebFrameClient(), UserAgentOverride(_))
      .WillOnce(Return(WebString()));
  EXPECT_TRUE(default_user_agent.Equals(UserAgent()));
}

}  // namespace
}  // namespace blink
