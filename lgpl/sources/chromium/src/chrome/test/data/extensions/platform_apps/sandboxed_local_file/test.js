// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('main.html', {}, function (createdWindow) {
    // 'main.html' is sandboxed, the new window should not be returned.
    if (createdWindow)
      chrome.test.notifyFail();
    else
      chrome.test.notifyPass();
  })
});
