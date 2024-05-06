#pragma once

#include <include/core/SkColorSpace.h>
#include <include/core/SkCanvas.h>
#include <include/ports/SkTypeface_win.h>
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/gl/GrGLAssembleInterface.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/gpu/ganesh/gl/GrGLBackendSurface.h>
#include <include/gpu/ganesh/gl/GrGLDirectContext.h>
#include <modules/skparagraph/include/FontCollection.h>
#include <modules/skparagraph/include/ParagraphBuilder.h>

#include "screen.hpp"

namespace rugui {

struct SkiaRenderer {
  int32_t fb_id = 0;
  int32_t fb_samples = 0;
  int32_t fb_stencil_bits = 0;

  sk_sp<GrDirectContext> context = nullptr;
  sk_sp<SkSurface> surface = nullptr;
  SkCanvas *canvas = nullptr;
  sk_sp<SkFontMgr> font_mgr = nullptr;
  sk_sp<skia::textlayout::FontCollection> font_collection = nullptr;

  bool show_debug_lines = false;

  auto init(Screen *screen) -> void;
  auto new_context() -> sk_sp<GrDirectContext>;
  auto new_surface(Screen *screen) -> sk_sp<SkSurface>;
  auto regenerate_surface(Screen *screen) -> void;

  auto clear(SkColor4f color) -> void;
  auto flush() -> void;
};

} // namespace gui
