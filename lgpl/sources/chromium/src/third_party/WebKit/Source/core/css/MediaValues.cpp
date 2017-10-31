// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/MediaValues.h"

#include "core/css/CSSHelper.h"
#include "core/css/MediaValuesCached.h"
#include "core/css/MediaValuesDynamic.h"
#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/LocalFrameView.h"
#include "core/frame/Settings.h"
#include "core/html/imports/HTMLImportsController.h"
#include "core/layout/LayoutObject.h"
#include "core/layout/api/LayoutViewItem.h"
#include "core/layout/compositing/PaintLayerCompositor.h"
#include "core/page/ChromeClient.h"
#include "core/page/Page.h"
#include "platform/graphics/ColorSpaceGamut.h"
#include "public/platform/WebScreenInfo.h"

namespace blink {

MediaValues* MediaValues::CreateDynamicIfFrameExists(LocalFrame* frame) {
  if (frame)
    return MediaValuesDynamic::Create(frame);
  return MediaValuesCached::Create();
}

double MediaValues::CalculateViewportWidth(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->View());
  DCHECK(frame->GetDocument());
  return frame->View()->ViewportSizeForMediaQueries().Width();
}

double MediaValues::CalculateViewportHeight(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->View());
  DCHECK(frame->GetDocument());
  return frame->View()->ViewportSizeForMediaQueries().Height();
}

int MediaValues::CalculateDeviceWidth(LocalFrame* frame) {
  DCHECK(frame && frame->View() && frame->GetSettings() && frame->GetPage());
  blink::WebScreenInfo screen_info =
      frame->GetPage()->GetChromeClient().GetScreenInfo();
  int device_width = screen_info.rect.width;
  if (frame->GetSettings()->GetReportScreenSizeInPhysicalPixelsQuirk())
    device_width = lroundf(device_width * screen_info.device_scale_factor);
  return device_width;
}

int MediaValues::CalculateDeviceHeight(LocalFrame* frame) {
  DCHECK(frame && frame->View() && frame->GetSettings() && frame->GetPage());
  blink::WebScreenInfo screen_info =
      frame->GetPage()->GetChromeClient().GetScreenInfo();
  int device_height = screen_info.rect.height;
  if (frame->GetSettings()->GetReportScreenSizeInPhysicalPixelsQuirk())
    device_height = lroundf(device_height * screen_info.device_scale_factor);
  return device_height;
}

bool MediaValues::CalculateStrictMode(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetDocument());
  return !frame->GetDocument()->InQuirksMode();
}

float MediaValues::CalculateDevicePixelRatio(LocalFrame* frame) {
  return frame->DevicePixelRatio();
}

int MediaValues::CalculateColorBitsPerComponent(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetPage());
  DCHECK(frame->GetPage()->MainFrame());
  if (!frame->GetPage()->MainFrame()->IsLocalFrame() ||
      frame->GetPage()->GetChromeClient().GetScreenInfo().is_monochrome)
    return 0;
  return frame->GetPage()
      ->GetChromeClient()
      .GetScreenInfo()
      .depth_per_component;
}

int MediaValues::CalculateMonochromeBitsPerComponent(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetPage());
  DCHECK(frame->GetPage()->MainFrame());
  if (!frame->GetPage()->MainFrame()->IsLocalFrame() ||
      !frame->GetPage()->GetChromeClient().GetScreenInfo().is_monochrome)
    return 0;
  return frame->GetPage()
      ->GetChromeClient()
      .GetScreenInfo()
      .depth_per_component;
}

int MediaValues::CalculateDefaultFontSize(LocalFrame* frame) {
  return frame->GetPage()->GetSettings().GetDefaultFontSize();
}

const String MediaValues::CalculateMediaType(LocalFrame* frame) {
  DCHECK(frame);
  if (!frame->View())
    return g_empty_atom;
  return frame->View()->MediaType();
}

WebDisplayMode MediaValues::CalculateDisplayMode(LocalFrame* frame) {
  DCHECK(frame);
  WebDisplayMode mode =
      frame->GetPage()->GetSettings().GetDisplayModeOverride();

  if (mode != kWebDisplayModeUndefined)
    return mode;

  if (!frame->View())
    return kWebDisplayModeBrowser;

  return frame->View()->DisplayMode();
}

bool MediaValues::CalculateThreeDEnabled(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(!frame->ContentLayoutItem().IsNull());
  DCHECK(frame->ContentLayoutItem().Compositor());
  bool three_d_enabled = false;
  if (LayoutViewItem view = frame->ContentLayoutItem())
    three_d_enabled = view.Compositor()->HasAcceleratedCompositing();
  return three_d_enabled;
}

PointerType MediaValues::CalculatePrimaryPointerType(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetPrimaryPointerType();
}

int MediaValues::CalculateAvailablePointerTypes(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetAvailablePointerTypes();
}

HoverType MediaValues::CalculatePrimaryHoverType(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetPrimaryHoverType();
}

int MediaValues::CalculateAvailableHoverTypes(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetAvailableHoverTypes();
}

DisplayShape MediaValues::CalculateDisplayShape(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetPage());
  return frame->GetPage()->GetChromeClient().GetScreenInfo().display_shape;
}

ColorSpaceGamut MediaValues::CalculateColorGamut(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetPage());
  return ColorSpaceUtilities::GetColorSpaceGamut(
      frame->GetPage()->GetChromeClient().GetScreenInfo());
}

bool MediaValues::ComputeLengthImpl(double value,
                                    CSSPrimitiveValue::UnitType type,
                                    unsigned default_font_size,
                                    double viewport_width,
                                    double viewport_height,
                                    double& result) {
  // The logic in this function is duplicated from
  // CSSToLengthConversionData::ZoomedComputedPixels() because
  // MediaValues::ComputeLength() needs nearly identical logic, but we haven't
  // found a way to make CSSToLengthConversionData::ZoomedComputedPixels() more
  // generic (to solve both cases) without hurting performance.
  // FIXME - Unite the logic here with CSSToLengthConversionData in a performant
  // way.
  switch (type) {
    case CSSPrimitiveValue::UnitType::kEms:
    case CSSPrimitiveValue::UnitType::kRems:
      result = value * default_font_size;
      return true;
    case CSSPrimitiveValue::UnitType::kPixels:
    case CSSPrimitiveValue::UnitType::kUserUnits:
      result = value;
      return true;
    case CSSPrimitiveValue::UnitType::kExs:
    // FIXME: We have a bug right now where the zoom will be applied twice to EX
    // units.
    case CSSPrimitiveValue::UnitType::kChs:
      // FIXME: We don't seem to be able to cache fontMetrics related values.
      // Trying to access them is triggering some sort of microtask. Serving the
      // spec's default instead.
      result = (value * default_font_size) / 2.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportWidth:
      result = (value * viewport_width) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportHeight:
      result = (value * viewport_height) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportMin:
      result = (value * std::min(viewport_width, viewport_height)) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportMax:
      result = (value * std::max(viewport_width, viewport_height)) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kCentimeters:
      result = value * kCssPixelsPerCentimeter;
      return true;
    case CSSPrimitiveValue::UnitType::kMillimeters:
      result = value * kCssPixelsPerMillimeter;
      return true;
    case CSSPrimitiveValue::UnitType::kInches:
      result = value * kCssPixelsPerInch;
      return true;
    case CSSPrimitiveValue::UnitType::kPoints:
      result = value * kCssPixelsPerPoint;
      return true;
    case CSSPrimitiveValue::UnitType::kPicas:
      result = value * kCssPixelsPerPica;
      return true;
    default:
      return false;
  }
}

LocalFrame* MediaValues::FrameFrom(Document& document) {
  Document* executing_document = document.ImportsController()
                                     ? document.ImportsController()->Master()
                                     : &document;
  DCHECK(executing_document);
  return executing_document->GetFrame();
}

}  // namespace blink
