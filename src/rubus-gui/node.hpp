#pragma once

#include <functional>
#include <vector>
#include <string>

#include <include/core/SkCanvas.h>
#include <modules/skparagraph/include/Paragraph.h>

#include "base.hpp"
#include "node_style.hpp"
#include "renderer.hpp"

namespace rugui {

struct Node {
public:
  enum class Traverse {
    SkipChildren,
    Continue,
    Break,
  };

  enum class Type {
    Rect,
    Text,
  };

  std::string name = "node";

  const Type type = Type::Rect;
  std::string text;
  std::unique_ptr<skia::textlayout::Paragraph> paragraph;

  Node *parent = nullptr;
  std::vector<Node *> children;

  NodeStyle style;
  UiNodeOutput output;

  std::function<void(Node *)> on_destroy;

  bool is_mouse_inside = false;
  bool is_mouse_over = false;
  bool is_mouse_down = false;

  std::function<void(Node *, int, int)> on_mouse_inside;
  std::function<void(Node *, int, int)> on_mouse_enter;
  std::function<void(Node *, int, int)> on_mouse_leave;
  std::function<bool(Node *, int, int)> on_mouse_over;
  std::function<bool(Node *, int, int)> on_mouse_out;
  std::function<bool(Node *, MouseButton, int, int)> on_mouse_down;
  std::function<bool(Node *, MouseButton, int, int)> on_mouse_up;
  std::function<bool(Node *, MouseButton, int, int)> on_mouse_click_in;
  std::function<bool(Node *, MouseButton, int, int)> on_mouse_click_out;

public:
  Node(std::string_view name) : name{name} {}
  Node(std::string_view name, NodeStyle style) : name{name}, style{style} {}
  Node(std::string_view name, std::string_view text) : name{name}, type{Type::Text}, text{text} {
    style.color = SkColors::kBlack;
    style.width = {SizeMode::FitContent, 0};
    style.height = {SizeMode::FitContent, 0};
    style.flex_dir = FlexDir::Row;
    style.flex_wrap = FlexWrap::Wrap;
  }

  ~Node() {
    if (on_destroy) {
      on_destroy(this);
    }
  }

  auto bfs(const std::function<Traverse(Node *)> &fn) -> void;
  auto dfs(const std::function<Traverse(Node *)> &fn) -> void;
  auto bfs_with_level(const std::function<Traverse(Node *, int)> &fn) -> void;
  auto dfs_with_level(const std::function<Traverse(Node *, int)> &fn) -> void;

  auto tree_str_repr() -> std::string;

  auto check_mouse_inside(int mouse_x, int mouse_y) -> bool;

  auto set_parent(Node *parent) -> Node *;
  auto add(Node *node) -> Node *;
  auto delete_all_children() -> void;

  auto set_style(NodeStyle style) -> Node *;

  auto set_display_mode(DisplayMode mode) -> Node *;
  auto set_local_transform(SkMatrix transform) -> Node *;
  auto set_screen_transform(SkMatrix transform) -> Node *;

  auto set_color(SkColor4f color) -> Node *;
  auto set_font_size(float size) -> Node *;

  auto set_vscroll(bool value) -> Node *;
  auto set_hscroll(bool value) -> Node *;

  auto set_clip_children(bool value) -> Node *;

  auto set_width(Size width) -> Node *;
  auto set_height(Size height) -> Node *;

  auto set_border_radius(float value) -> Node *;
  auto set_border_radius_tl(float value) -> Node *;
  auto set_border_radius_tr(float value) -> Node *;
  auto set_border_radius_br(float value) -> Node *;
  auto set_border_radius_bl(float value) -> Node *;

  auto set_margin(float value) -> Node *;
  auto set_margin_row(float value) -> Node *;
  auto set_margin_col(float value) -> Node *;
  auto set_margin_t(float value) -> Node *;
  auto set_margin_b(float value) -> Node *;
  auto set_margin_r(float value) -> Node *;
  auto set_margin_l(float value) -> Node *;

  auto set_padding(float value) -> Node *;
  auto set_padding_row(float value) -> Node *;
  auto set_padding_col(float value) -> Node *;
  auto set_padding_t(float value) -> Node *;
  auto set_padding_b(float value) -> Node *;
  auto set_padding_r(float value) -> Node *;
  auto set_padding_l(float value) -> Node *;

  auto set_flex_dir(FlexDir flex_dir) -> Node *;
  auto set_flex_wrap(FlexWrap flex_wrap) -> Node *;
  auto set_flex_align(FlexAlign align) -> Node *;
  auto set_flex_items_align(FlexAlign align) -> Node *;
  auto set_flex_content_align(FlexAlign align) -> Node *;
  auto set_flex_self_align(FlexAlign align) -> Node *;

  auto set_on_mouse_inside(const std::function<void(Node *, int, int)> &fn) -> Node *;
  auto set_on_mouse_enter(const std::function<void(Node *, int, int)> &fn) -> Node *;
  auto set_on_mouse_leave(const std::function<void(Node *, int, int)> &fn) -> Node *;
  auto set_on_mouse_over(const std::function<bool(Node *, int, int)> &fn) -> Node *;
  auto set_on_mouse_out(const std::function<bool(Node *, int, int)> &fn) -> Node *;
  auto set_on_mouse_down(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node *;
  auto set_on_mouse_up(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node *;
  auto set_on_mouse_click_in(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node *;
  auto set_on_mouse_click_out(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node *;

  auto layout_pass1() -> std::vector<Node *>;
  auto layout_pass2(std::span<Node *> reverse_dfs_nodes, SkiaRenderer *renderer) -> void;
  auto layout_pass3() -> void;
  auto layout_pass4(std::span<Node *> reverse_dfs_nodes) -> void;
  auto layout_pass5() -> void;
  auto layout(SkiaRenderer *renderer) -> void;

  auto run_mouse_enter_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_leave_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_over_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_leave_out_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_down_event(MouseButton button, int mouse_x, int mouse_y) -> void;
  auto run_mouse_up_event(MouseButton button, int mouse_x, int mouse_y) -> void;
  auto run_mouse_click_in_event(MouseButton button, int mouse_x, int mouse_y) -> void;
  auto run_mouse_click_out_event(MouseButton button, int mouse_x, int mouse_y) -> void;

  auto calculate_screen_transform() -> void;

  auto draw(SkiaRenderer *renderer) -> void;
  auto draw_all(SkiaRenderer *renderer) -> void;
};

} // namespace rugui
