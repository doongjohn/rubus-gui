#include "renderer.hpp"

#include <iostream>

#include <glad/glad.h>
#include <include/core/SkGraphics.h>

namespace rugui {

auto SkiaRenderer::init(Screen *screen) -> void {
  context = new_context();
  if (context == nullptr) {
    return;
  }

  surface = new_surface(screen);
  if (surface == nullptr) {
    return;
  }
  canvas = surface->getCanvas();

  font_mgr = SkFontMgr_New_DirectWrite();
  if (font_mgr == nullptr) {
    return;
  }
  font_collection = sk_sp{new skia::textlayout::FontCollection{}};
  font_collection->setDefaultFontManager(font_mgr);
}

auto SkiaRenderer::new_context() -> sk_sp<GrDirectContext> {
  auto interface = GrGLMakeNativeInterface();
  if (interface == nullptr) {
    std::cout << "skia: interface is null!\n";
    return nullptr;
  }

  auto context_opts = GrContextOptions{};
  auto sk_context = GrDirectContexts::MakeGL(interface, context_opts);
  if (sk_context == nullptr) {
    std::cout << "skia: context is null!\n";
    return nullptr;
  }

  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb_id);
  glGetIntegerv(GL_SAMPLES, &fb_samples);
  glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
                                        &fb_stencil_bits);

  return sk_context;
}

auto SkiaRenderer::new_surface(Screen *screen) -> sk_sp<SkSurface> {
  auto fb_info = GrGLFramebufferInfo{};
  fb_info.fFBOID = fb_id;
  fb_info.fFormat = GL_SRGB8_ALPHA8;

  auto render_target =
    GrBackendRenderTargets::MakeGL(screen->width, screen->height, fb_samples, fb_stencil_bits, fb_info);
  if (!render_target.isValid()) {
    std::cout << "skia: render target is invalid!\n";
    return nullptr;
  }

  auto color_type = SkColorType::kSRGBA_8888_SkColorType;
  auto sk_surface = SkSurfaces::WrapBackendRenderTarget(context.get(), render_target, kBottomLeft_GrSurfaceOrigin,
                                                        color_type, SkColorSpace::MakeSRGBLinear(), nullptr);
  if (sk_surface == nullptr) {
    std::cout << "skia: sk_surface is null!\n";
    return nullptr;
  }

  return sk_surface;
}

auto SkiaRenderer::regenerate_surface(Screen *screen) -> void {
  if (surface == nullptr) {
    return;
  }
  if (surface->width() != screen->width || surface->height() != screen->height) {
    surface = new_surface(screen);
    canvas = surface->getCanvas();
  }
}

auto SkiaRenderer::clear(SkColor4f color) -> void {
  canvas->clear(color);
}

auto SkiaRenderer::flush() -> void {
  context->flush();
}

} // namespace rugui
