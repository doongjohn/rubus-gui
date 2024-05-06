#pragma once

#include <span>
#include <vector>

#include <include/core/SkPoint.h>
#include <include/core/SkSize.h>
#include <include/core/SkMatrix.h>
#include <include/core/SkColor.h>

namespace rugui {

enum class DisplayMode {
  Shown,     // Do layout and drawing. (default)
  Hidden,    // Do layout but skip drawing.
  Collapsed, // Skip layout and drawing.
};

enum class TransformMode {
  Local,
  Screen,
};

enum class SizeMode {
  Self,       // Set `this.rect_size` to `Size.value`.
  Parent,     // Set `this.rect_size` to `parent.rect_size * Size.value`.
  FitContent, // Set `this.rect_size` to children total size. (ignores `Size.value`)
};

struct Size {
  SizeMode mode = SizeMode::Self;
  float value = 0;
};

enum class FlexDir {
  Row, // Order children from top to bottom. (default)
  Col, // Order children from left to right.
};

enum class FlexWrap {
  NoWrap, // Don't wrap. (default)
  Wrap,   // Wrap child node if it overflows the container.
};

enum class FlexAlign {
  Inherit,
  Start,
  Center,
  End,
};

struct NodeStyle {
  DisplayMode display_mode = DisplayMode::Shown;

  TransformMode transform_mode = TransformMode::Local;
  SkMatrix transform = SkMatrix::I();
  SkMatrix screen_transform = SkMatrix::I();

  SkColor4f color = SkColors::kTransparent;

  float font_size = 20.f;

  bool is_vscroll_enabled = true;
  bool is_hscroll_enabled = true;
  float vscroll_amount = 0;
  float hscroll_amount = 0;

  bool is_clip_enabled = true;
  SkRect clip_rect = SkRect::MakeEmpty(); // (screen space)

  Size width;
  Size height;
  float border_radius_tl = 0;
  float border_radius_tr = 0;
  float border_radius_br = 0;
  float border_radius_bl = 0;
  float margin_t = 0;
  float margin_b = 0;
  float margin_r = 0;
  float margin_l = 0;
  float padding_t = 0;
  float padding_b = 0;
  float padding_r = 0;
  float padding_l = 0;

  FlexDir flex_dir = FlexDir::Col;
  FlexWrap flex_wrap = FlexWrap::NoWrap;
  FlexAlign flex_align = FlexAlign::Start;
  FlexAlign flex_items_align = FlexAlign::Start;
  FlexAlign flex_content_align = FlexAlign::Start;
  FlexAlign flex_self_align = FlexAlign::Inherit;

  auto set_display_mode(DisplayMode mode) -> NodeStyle &;
  auto set_local_transform(SkMatrix transform) -> NodeStyle &;
  auto set_screen_transform(SkMatrix transform) -> NodeStyle &;

  auto set_color(SkColor4f color) -> NodeStyle &;
  auto set_font_size(float size) -> NodeStyle &;

  auto set_vscroll(bool value) -> NodeStyle &;
  auto set_hscroll(bool value) -> NodeStyle &;

  auto set_clip_children(bool value) -> NodeStyle &;

  auto set_width(Size width) -> NodeStyle &;
  auto set_height(Size height) -> NodeStyle &;

  auto set_border_radius(float value) -> NodeStyle &;
  auto set_border_radius_tl(float value) -> NodeStyle &;
  auto set_border_radius_tr(float value) -> NodeStyle &;
  auto set_border_radius_br(float value) -> NodeStyle &;
  auto set_border_radius_bl(float value) -> NodeStyle &;

  auto set_margin(float value) -> NodeStyle &;
  auto set_margin_col(float value) -> NodeStyle &;
  auto set_margin_row(float value) -> NodeStyle &;
  auto set_margin_t(float value) -> NodeStyle &;
  auto set_margin_b(float value) -> NodeStyle &;
  auto set_margin_r(float value) -> NodeStyle &;
  auto set_margin_l(float value) -> NodeStyle &;

  auto set_padding(float value) -> NodeStyle &;
  auto set_padding_col(float value) -> NodeStyle &;
  auto set_padding_row(float value) -> NodeStyle &;
  auto set_padding_t(float value) -> NodeStyle &;
  auto set_padding_b(float value) -> NodeStyle &;
  auto set_padding_r(float value) -> NodeStyle &;
  auto set_padding_l(float value) -> NodeStyle &;

  auto set_flex_dir(FlexDir flex_dir) -> NodeStyle &;
  auto set_flex_wrap(FlexWrap flex_wrap) -> NodeStyle &;
  auto set_flex_align(FlexAlign align) -> NodeStyle &;
  auto set_flex_items_align(FlexAlign align) -> NodeStyle &;
  auto set_flex_content_align(FlexAlign align) -> NodeStyle &;
  auto set_flex_self_align(FlexAlign align) -> NodeStyle &;
};

struct FlexLine {
  float width = 0;
  float height = 0;
  std::span<struct Node *> items;
};

struct UiNodeOutput {
  NodeStyle style;
  SkPoint pos = {0, 0};
  SkSize rect_size = {0, 0};
  SkSize layout_size = {0, 0};
  SkSize content_size = {0, 0};
  SkSize content_overflow = {0, 0};
  SkSize max_content_area = {0, 0};
  std::vector<FlexLine> flex_lines;

  UiNodeOutput() = default;
  explicit UiNodeOutput(NodeStyle style) : style{style} {}

  auto get_rect_pos() -> SkPoint;
  auto get_content_area() -> SkSize;

  auto get_margin_row() -> float;
  auto get_margin_col() -> float;

  auto get_padding_row() -> float;
  auto get_padding_col() -> float;

  auto calculate_flex_lines(Node *node) -> void;
};

} // namespace rugui
