// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "public/web/WebSelection.h"

#include "core/editing/SelectionType.h"
#include "core/layout/compositing/CompositedSelection.h"

namespace blink {

static WebSelectionBound getWebSelectionBound(
    const CompositedSelection& selection,
    bool isStart) {
  DCHECK_NE(selection.type, NoSelection);
  const CompositedSelectionBound& bound =
      isStart ? selection.start : selection.end;
  DCHECK(bound.layer);

  WebSelectionBound::Type type = WebSelectionBound::Caret;
  if (selection.type == RangeSelection) {
    if (isStart)
      type = bound.isTextDirectionRTL ? WebSelectionBound::SelectionRight
                                      : WebSelectionBound::SelectionLeft;
    else
      type = bound.isTextDirectionRTL ? WebSelectionBound::SelectionLeft
                                      : WebSelectionBound::SelectionRight;
  }

  WebSelectionBound result(type);
  result.layerId = bound.layer->platformLayer()->id();
  result.edgeTopInLayer = roundedIntPoint(bound.edgeTopInLayer);
  result.edgeBottomInLayer = roundedIntPoint(bound.edgeBottomInLayer);
  result.isTextDirectionRTL = bound.isTextDirectionRTL;
  return result;
}

// SelectionType enums have the same values; enforced in
// AssertMatchingEnums.cpp.
WebSelection::WebSelection(const CompositedSelection& selection)
    : m_selectionType(static_cast<WebSelection::SelectionType>(selection.type)),
      m_start(getWebSelectionBound(selection, true)),
      m_end(getWebSelectionBound(selection, false)),
      m_boundingRect(selection.boundingRect) {}

WebSelection::WebSelection(const WebSelection& other)
    : m_selectionType(other.m_selectionType),
      m_start(other.m_start),
      m_end(other.m_end),
      m_boundingRect(other.m_boundingRect) {}

}  // namespace blink
