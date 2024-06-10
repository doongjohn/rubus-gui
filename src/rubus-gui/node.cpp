#include "node.hpp"

#include <ranges>
#include <array>
#include <stack>
#include <queue>
#include <format>

#include <include/core/SkRRect.h>

namespace rugui {

auto Node::bfs(const std::function<Traverse(Node *)> &fn) -> void {
  auto queue = std::queue<Node *>{};
  queue.emplace(this);
  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();
    auto result = fn(node);
    if (result == Traverse::Break) {
      break;
    }
    if (result == Traverse::SkipChildren) {
      continue;
    }
    for (auto child : node->children) {
      queue.emplace(child);
    }
  }
}

auto Node::dfs(const std::function<Traverse(Node *)> &fn) -> void {
  auto stack = std::stack<Node *>{};
  stack.emplace(this);
  while (!stack.empty()) {
    auto node = stack.top();
    stack.pop();
    auto result = fn(node);
    if (result == Traverse::Break) {
      break;
    }
    if (result == Traverse::SkipChildren) {
      continue;
    }
    for (auto child : node->children | std::ranges::views::reverse) {
      stack.emplace(child);
    }
  }
}

auto Node::bfs_with_level(const std::function<Traverse(Node *, int)> &fn) -> void {
  auto level = 0;
  auto queue = std::queue<Node *>{};
  queue.emplace(this);
  while (!queue.empty()) {
    auto level_size = queue.size();
    while (level_size-- != 0) {
      auto node = queue.front();
      queue.pop();
      auto result = fn(node, level);
      if (result == Traverse::Break) {
        break;
      }
      if (result == Traverse::SkipChildren) {
        continue;
      }
      for (auto child : node->children) {
        queue.emplace(child);
      }
    }
    ++level;
  }
}

auto Node::dfs_with_level(const std::function<Traverse(Node *, int)> &fn) -> void {
  auto stack = std::stack<std::tuple<Node *, int>>{};
  stack.emplace(this, 0);
  while (!stack.empty()) {
    auto [node, level] = stack.top();
    stack.pop();
    auto result = fn(node, level);
    if (result == Traverse::Break) {
      break;
    }
    if (result == Traverse::SkipChildren) {
      continue;
    }
    for (auto child : node->children | std::ranges::views::reverse) {
      stack.emplace(child, level + 1);
    }
  }
}

auto Node::tree_str_repr() -> std::string {
  auto str = std::string{};
  dfs_with_level([&](Node *node, int level) -> Traverse {
    for (auto i = 0; i < level; ++i) {
      str.append("|  ");
    }
    str.append(std::format("└> \"{}\"\n", node->name));
    return Traverse::Continue;
  });
  return str;
}

auto Node::check_mouse_inside(int mouse_x, int mouse_y) -> bool {
  auto is_in_clip = true;
  if (output.style.is_clip_enabled && parent != nullptr) {
    auto pos = parent->output.get_rect_pos();
    auto rect = SkRect::MakeXYWH(pos.fX, pos.fY, parent->output.rect_size.fWidth, parent->output.rect_size.fHeight);
    rect = parent->output.style.screen_transform.mapRect(rect);
    is_in_clip = rect.contains((float)mouse_x, (float)mouse_y);
  }

  auto is_in_self = false;
  {
    auto pos = output.get_rect_pos();
    auto rect = SkRect::MakeXYWH(pos.fX, pos.fY, output.rect_size.fWidth, output.rect_size.fHeight);
    rect = output.style.screen_transform.mapRect(rect);
    is_in_self = rect.contains((float)mouse_x, (float)mouse_y);
  }

  return is_in_clip && is_in_self;
}

auto Node::set_parent(Node *parent) -> Node * {
  this->parent = parent;
  return this;
}

auto Node::add(Node *node) -> Node * {
  if (type != Type::Text) {
    node->parent = this;
    children.push_back(node);
  }
  return this;
}

auto Node::delete_all_children() -> void {
  auto stack = std::stack<Node *>{};
  for (auto child : children) {
    stack.emplace(child);
  }
  while (!stack.empty()) {
    auto node = stack.top();
    stack.pop();
    for (auto child : node->children) {
      stack.emplace(child);
    }
    delete node;
  }
  children.clear();
}

auto Node::set_style(NodeStyle style) -> Node * {
  this->style = style;
  return this;
}

auto Node::set_display_mode(DisplayMode mode) -> Node * {
  this->style.set_display_mode(mode);
  return this;
}

auto Node::set_local_transform(SkMatrix transform) -> Node * {
  this->style.set_local_transform(transform);
  return this;
}

auto Node::set_screen_transform(SkMatrix transform) -> Node * {
  this->style.set_screen_transform(transform);
  return this;
}

auto Node::set_color(SkColor4f color) -> Node * {
  this->style.set_color(color);
  return this;
}

auto Node::set_font_size(float size) -> Node * {
  this->style.set_font_size(size);
  return this;
}

auto Node::set_image(sk_sp<SkImage> image) -> Node * {
  this->style.set_image(image);
  return this;
}

auto Node::set_image_sampling(SkSamplingOptions image_sampling) -> Node * {
  this->style.set_image_sampling(image_sampling);
  return this;
}

auto Node::set_vscroll(bool value) -> Node * {
  this->style.set_vscroll(value);
  return this;
}

auto Node::set_hscroll(bool value) -> Node * {
  this->style.set_hscroll(value);
  return this;
}

auto Node::set_clip_children(bool value) -> Node * {
  this->style.set_clip_children(value);
  return this;
}

auto Node::set_width(Size width) -> Node * {
  this->style.set_width(width);
  return this;
}

auto Node::set_height(Size height) -> Node * {
  this->style.set_height(height);
  return this;
}

auto Node::set_border_radius(float value) -> Node * {
  this->style.set_border_radius(value);
  return this;
}

auto Node::set_border_radius_tl(float value) -> Node * {
  this->style.set_border_radius_tl(value);
  return this;
}

auto Node::set_border_radius_tr(float value) -> Node * {
  this->style.set_border_radius_tr(value);
  return this;
}

auto Node::set_border_radius_br(float value) -> Node * {
  this->style.set_border_radius_br(value);
  return this;
}

auto Node::set_border_radius_bl(float value) -> Node * {
  this->style.set_border_radius_bl(value);
  return this;
}

auto Node::set_margin(float value) -> Node * {
  this->style.set_margin(value);
  return this;
}

auto Node::set_margin_row(float value) -> Node * {
  this->style.set_margin_row(value);
  return this;
}

auto Node::set_margin_col(float value) -> Node * {
  this->style.set_margin_col(value);
  return this;
}

auto Node::set_margin_t(float value) -> Node * {
  this->style.set_margin_t(value);
  return this;
}

auto Node::set_margin_b(float value) -> Node * {
  this->style.set_margin_b(value);
  return this;
}

auto Node::set_margin_r(float value) -> Node * {
  this->style.set_margin_r(value);
  return this;
}

auto Node::set_margin_l(float value) -> Node * {
  this->style.set_margin_l(value);
  return this;
}

auto Node::set_padding(float value) -> Node * {
  this->style.set_padding(value);
  return this;
}

auto Node::set_padding_row(float value) -> Node * {
  this->style.set_padding_row(value);
  return this;
}

auto Node::set_padding_col(float value) -> Node * {
  this->style.set_padding_col(value);
  return this;
}

auto Node::set_padding_t(float value) -> Node * {
  this->style.set_padding_t(value);
  return this;
}

auto Node::set_padding_b(float value) -> Node * {
  this->style.set_padding_b(value);
  return this;
}

auto Node::set_padding_r(float value) -> Node * {
  this->style.set_padding_r(value);
  return this;
}

auto Node::set_padding_l(float value) -> Node * {
  this->style.set_padding_l(value);
  return this;
}

auto Node::set_flex_dir(FlexDir flex_dir) -> Node * {
  this->style.set_flex_dir(flex_dir);
  return this;
}

auto Node::set_flex_wrap(FlexWrap flex_wrap) -> Node * {
  this->style.set_flex_wrap(flex_wrap);
  return this;
}

auto Node::set_flex_align(FlexAlign align) -> Node * {
  this->style.set_flex_align(align);
  return this;
}

auto Node::set_flex_items_align(FlexAlign align) -> Node * {
  this->style.set_flex_items_align(align);
  return this;
}

auto Node::set_flex_content_align(FlexAlign align) -> Node * {
  this->style.set_flex_content_align(align);
  return this;
}

auto Node::set_flex_self_align(FlexAlign align) -> Node * {
  this->style.set_flex_self_align(align);
  return this;
}

auto Node::set_on_mouse_inside(const std::function<void(Node *, int, int)> &fn) -> Node * {
  on_mouse_inside = fn;
  return this;
}

auto Node::set_on_mouse_enter(const std::function<void(Node *, int, int)> &fn) -> Node * {
  on_mouse_enter = fn;
  return this;
}

auto Node::set_on_mouse_leave(const std::function<void(Node *, int, int)> &fn) -> Node * {
  on_mouse_leave = fn;
  return this;
}

auto Node::set_on_mouse_over(const std::function<bool(Node *, int, int)> &fn) -> Node * {
  on_mouse_over = fn;
  return this;
}

auto Node::set_on_mouse_out(const std::function<bool(Node *, int, int)> &fn) -> Node * {
  on_mouse_out = fn;
  return this;
}

auto Node::set_on_mouse_down(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node * {
  on_mouse_down = fn;
  return this;
}

auto Node::set_on_mouse_up(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node * {
  on_mouse_up = fn;
  return this;
}

auto Node::set_on_mouse_click_in(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node * {
  on_mouse_click_in = fn;
  return this;
}

auto Node::set_on_mouse_click_out(const std::function<bool(Node *, MouseButton, int, int)> &fn) -> Node * {
  on_mouse_click_out = fn;
  return this;
}

auto Node::layout_pass1() -> std::vector<Node *> {
  auto reverse_dfs_nodes = std::vector<Node *>{};

  // Pass 1:
  // - Initialize the output based on style.
  // - Determine basic size.
  // - Determine max content area.
  dfs([&](Node *node) -> Traverse {
    reverse_dfs_nodes.push_back(node);

    const auto parent = node->parent;
    node->output = UiNodeOutput{node->style};

    // determine flex align
    if (node->output.style.flex_align == FlexAlign::Inherit) {
      node->output.style.flex_align = parent == nullptr ? FlexAlign::Start : parent->output.style.flex_align;
    }

    // determine flex items align
    if (node->output.style.flex_items_align == FlexAlign::Inherit) {
      node->output.style.flex_items_align =
        parent == nullptr ? FlexAlign::Start : parent->output.style.flex_items_align;
    }

    // determine flex self align
    if (node->output.style.flex_self_align == FlexAlign::Inherit) {
      node->output.style.flex_self_align = parent == nullptr ? FlexAlign::Start : parent->output.style.flex_items_align;
    }

    // skip collapsed node
    if (node->style.display_mode == DisplayMode::Collapsed) {
      node->output.layout_size = {0, 0};
      node->output.rect_size = {0, 0};
      node->output.style.width = {SizeMode::Self, 0};
      node->output.style.height = {SizeMode::Self, 0};
      return Traverse::SkipChildren;
    }

    const auto margin_row = node->output.get_margin_row();
    const auto margin_col = node->output.get_margin_col();

    // determine width
    switch (node->output.style.width.mode) {
    case SizeMode::Self:
      node->output.layout_size.fWidth = node->output.style.width.value + margin_row;
      node->output.rect_size.fWidth = node->output.style.width.value;
      break;
    case SizeMode::Parent: {
      if (parent != nullptr) {
        node->output.layout_size.fWidth =
          (parent->output.rect_size.fWidth - parent->output.get_padding_row()) * node->output.style.width.value;
        node->output.rect_size.fWidth = node->output.layout_size.fWidth - margin_row;
      }
    } break;
    case SizeMode::FitContent:
      node->output.layout_size.fWidth = 0;
      node->output.rect_size.fWidth = 0;
      break;
    }

    // determine height
    switch (node->output.style.height.mode) {
    case SizeMode::Self:
      node->output.layout_size.fHeight = node->output.style.height.value + margin_col;
      node->output.rect_size.fHeight = node->output.style.height.value;
      break;
    case SizeMode::Parent: {
      if (parent != nullptr) {
        node->output.layout_size.fHeight =
          (parent->output.rect_size.fHeight - parent->output.get_padding_col()) * node->output.style.height.value;
        node->output.rect_size.fHeight = node->output.layout_size.fHeight - margin_col;
      }
    } break;
    case SizeMode::FitContent:
      node->output.layout_size.fHeight = 0;
      node->output.rect_size.fHeight = 0;
      break;
    }

    // determine max content area
    if (parent != nullptr) {
      node->output.max_content_area = parent->output.get_content_area();
      node->output.max_content_area.fWidth -= margin_row + node->output.get_padding_row();
      node->output.max_content_area.fHeight -= margin_col + node->output.get_padding_col();
    } else {
      node->output.max_content_area = node->output.get_content_area();
    }

    return Traverse::Continue;
  });

  return reverse_dfs_nodes;
}

auto Node::layout_pass2(std::span<Node *> reverse_dfs_nodes, SkiaRenderer *renderer) -> void {
  // Pass 2:
  // - Determine initial FitContent size.
  // - Determine initial content size.
  for (auto node : reverse_dfs_nodes | std::ranges::views::reverse) {
    if (node->output.style.display_mode == DisplayMode::Collapsed) {
      continue;
    }

    auto &content_width = node->output.content_size.fWidth;
    auto &content_height = node->output.content_size.fHeight;

    switch (node->type) {
    case Type::Rect: {
      for (const auto child : node->children) {
        switch (node->output.style.flex_dir) {
        case FlexDir::Row:
          content_width += child->output.layout_size.fWidth;
          if (content_height < child->output.layout_size.fHeight) {
            content_height = child->output.layout_size.fHeight;
          }
          break;
        case FlexDir::Col:
          if (content_width < child->output.layout_size.fWidth) {
            content_width = child->output.layout_size.fWidth;
          }
          content_height += child->output.layout_size.fHeight;
          break;
        }
      }
    } break;
    case Type::Text: {
      auto paint = SkPaint{node->output.style.color};
      paint.setAntiAlias(true);

      auto text_style = skia::textlayout::TextStyle{};
      text_style.setFontSize(node->output.style.font_size);
      text_style.setForegroundPaint(paint);

      auto builder =
        skia::textlayout::ParagraphBuilder::make(skia::textlayout::ParagraphStyle{}, renderer->font_collection);
      builder->pushStyle(text_style);
      builder->addText(node->text.data(), node->text.length());

      node->paragraph = builder->Build();
      node->paragraph->layout(std::numeric_limits<float>::infinity());
      content_width = node->paragraph->getMaxIntrinsicWidth();
      content_height = node->paragraph->getHeight();
    } break;
    }

    // width
    if (node->output.style.width.mode == SizeMode::FitContent) {
      const auto margin_row = node->output.get_margin_row();
      const auto padding_row = node->output.get_padding_row();
      node->output.layout_size.fWidth = content_width + margin_row + padding_row;
      node->output.rect_size.fWidth = content_width + padding_row;
    }
    // height
    if (node->output.style.height.mode == SizeMode::FitContent) {
      const auto margin_col = node->output.get_margin_col();
      const auto padding_col = node->output.get_padding_col();
      node->output.layout_size.fHeight = content_height + margin_col + padding_col;
      node->output.rect_size.fHeight = content_height + padding_col;
    }
  }
}

auto Node::layout_pass3() -> void {
  // TODO: determine perpendicular fit wrap first

  auto fit_wrap_nodes = std::vector<Node *>{};
  auto fit_wrap_total_size = 0.f;
  auto avaliable_size = 0.f;

  // Pass 3:
  // - Determine flex container size for Wrap + FitContent.
  bfs([&](Node *node) -> Traverse {
    const auto parent = node->parent;
    if (parent == nullptr) {
      return Traverse::Continue;
    }

    auto &output = node->output;

    // determine max content area
    output.max_content_area = parent->output.get_content_area();
    output.max_content_area.fWidth -= output.get_margin_row() + output.get_padding_row();
    output.max_content_area.fHeight -= output.get_margin_col() + output.get_padding_col();

    // subtract overflow
    if (output.style.flex_wrap == FlexWrap::Wrap) {
      // width
      if (output.style.width.mode == SizeMode::FitContent) {
        if (output.content_size.fWidth > output.max_content_area.fWidth) {
          auto overflow = output.content_size.fWidth - output.max_content_area.fWidth;
          output.layout_size.fWidth -= overflow;
          output.rect_size.fWidth -= overflow;
        }
      }
      // height
      if (output.style.height.mode == SizeMode::FitContent) {
        if (output.content_size.fHeight > output.max_content_area.fHeight) {
          auto overflow = output.content_size.fHeight - output.max_content_area.fHeight;
          output.layout_size.fHeight -= overflow;
          output.rect_size.fHeight -= overflow;
        }
      }
    }

    // set initial avaliable_size
    if (node == parent->children.front()) {
      fit_wrap_nodes.clear();
      switch (parent->output.style.flex_dir) {
      case FlexDir::Row:
        avaliable_size = parent->output.max_content_area.fWidth;
        break;
      case FlexDir::Col:
        avaliable_size = parent->output.max_content_area.fHeight;
        break;
      }
    }

    // update avaliable_size
    switch (parent->output.style.flex_dir) {
    case FlexDir::Row:
      if (output.style.flex_dir == FlexDir::Row && output.style.flex_wrap == FlexWrap::Wrap &&
          output.style.width.mode == SizeMode::FitContent) {
        fit_wrap_nodes.push_back(node);
        fit_wrap_total_size += output.content_size.fWidth;
      } else {
        avaliable_size -= output.layout_size.fWidth;
        if (avaliable_size < 0) {
          avaliable_size = 0;
        }
      }
      break;
    case FlexDir::Col:
      if (output.style.flex_dir == FlexDir::Col && output.style.flex_wrap == FlexWrap::Wrap &&
          output.style.height.mode == SizeMode::FitContent) {
        fit_wrap_nodes.push_back(node);
        fit_wrap_total_size += output.content_size.fHeight;
      } else {
        avaliable_size -= output.layout_size.fHeight;
        if (avaliable_size < 0) {
          avaliable_size = 0;
        }
      }
      break;
    }

    if (node == parent->children.back()) {
      // determine flex container size
      for (const auto fit_wrap_node : fit_wrap_nodes) {
        switch (parent->output.style.flex_dir) {
        case FlexDir::Row: {
          auto layout_width = (fit_wrap_node->output.content_size.fWidth / fit_wrap_total_size) * avaliable_size;
          fit_wrap_node->output.layout_size.fWidth = layout_width;
          fit_wrap_node->output.rect_size.fWidth = fit_wrap_node->output.layout_size.fWidth -
                                                   fit_wrap_node->output.style.margin_l -
                                                   fit_wrap_node->output.style.margin_r;
        } break;
        case FlexDir::Col: {
          auto layout_height = (fit_wrap_node->output.content_size.fHeight / fit_wrap_total_size) * avaliable_size;
          fit_wrap_node->output.layout_size.fHeight = layout_height;
          fit_wrap_node->output.rect_size.fHeight = fit_wrap_node->output.layout_size.fHeight -
                                                    fit_wrap_node->output.style.margin_t -
                                                    fit_wrap_node->output.style.margin_b;
        } break;
        }
      }
    }

    if (node->output.style.display_mode == DisplayMode::Collapsed) {
      return Traverse::SkipChildren;
    }
    return Traverse::Continue;
  });
}

auto Node::layout_pass4(std::span<Node *> reverse_dfs_nodes) -> void {
  // Pass 4:
  // - Calculate flex lines.
  // - Determine final size.
  for (auto node : reverse_dfs_nodes | std::ranges::views::reverse) {
    if (node->output.style.display_mode == DisplayMode::Collapsed) {
      continue;
    }
    if (node->type == Type::Rect && node->children.empty()) {
      continue;
    }

    node->output.calculate_flex_lines(node);

    // width
    if (node->output.style.width.mode == SizeMode::FitContent) {
      if (node->output.style.flex_dir == FlexDir::Col ||
          (node->output.style.flex_dir == FlexDir::Row &&
           node->output.get_content_area().fWidth > node->output.content_size.fWidth)) {
        const auto margin_row = node->output.get_margin_row();
        const auto padding_row = node->output.get_padding_row();
        node->output.layout_size.fWidth = node->output.content_size.fWidth + margin_row + padding_row;
        node->output.rect_size.fWidth = node->output.content_size.fWidth + padding_row;
      }
    }
    // height
    if (node->output.style.height.mode == SizeMode::FitContent) {
      if (node->output.style.flex_dir == FlexDir::Row ||
          (node->output.style.flex_dir == FlexDir::Col &&
           node->output.get_content_area().fHeight > node->output.content_size.fHeight)) {
        const auto margin_col = node->output.get_margin_col();
        const auto padding_col = node->output.get_padding_col();
        node->output.layout_size.fHeight = node->output.content_size.fHeight + margin_col + padding_col;
        node->output.rect_size.fHeight = node->output.content_size.fHeight + padding_col;
      }
    }

    // switch (node->output.style.flex_dir) {
    // case FlexDirection::Row:
    //   // height
    //   if (node->output.style.height.mode == SizeMode::FitContent) {
    //     const auto margin_col = node->output.get_margin_col();
    //     const auto padding_col = node->output.get_padding_col();
    //     node->output.layout_size.fHeight = node->output.content_size.fHeight + margin_col + padding_col;
    //     node->output.rect_size.fHeight = node->output.content_size.fHeight + padding_col;
    //   }
    //   break;
    // case FlexDirection::Col:
    //   // width
    //   if (node->output.style.width.mode == SizeMode::FitContent) {
    //     const auto margin_row = node->output.get_margin_row();
    //     const auto padding_row = node->output.get_padding_row();
    //     node->output.layout_size.fWidth = node->output.content_size.fWidth + margin_row + padding_row;
    //     node->output.rect_size.fWidth = node->output.content_size.fWidth + padding_row;
    //   }
    //   break;
    // }
  }
}

auto Node::layout_pass5() -> void {
  // Pass 5:
  // - Set position of nodes based on the alignment settings.
  bfs([&](Node *node) -> Traverse {
    const auto parent = node->parent;
    if (parent == nullptr) {
      return Traverse::Continue;
    }

    // set initial position
    node->output.pos.fX = parent->output.pos.fX;
    node->output.pos.fY = parent->output.pos.fY;

    if (node == parent->children.back()) {
      auto line_offset_x = 0.f;
      auto line_offset_y = 0.f;

      for (const auto &line : parent->output.flex_lines) {
        auto align_offset_x = 0.f;
        auto align_offset_y = 0.f;

        const auto starting_pos_x = parent->output.style.margin_l + parent->output.style.padding_l;
        const auto starting_pos_y = parent->output.style.margin_t + parent->output.style.padding_t;

        // flex align (main axis align)
        {
          const auto remaining_width = parent->output.get_content_area().fWidth - line.width;
          const auto remaining_height = parent->output.get_content_area().fHeight - line.height;

          switch (parent->output.style.flex_align) {
          case FlexAlign::Start: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_x = starting_pos_x;
              break;
            case FlexDir::Col:
              align_offset_y = starting_pos_y;
              break;
            }
          } break;
          case FlexAlign::Center: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_x = starting_pos_x + remaining_width / 2.f;
              break;
            case FlexDir::Col:
              align_offset_y = starting_pos_y + remaining_height / 2.f;
              break;
            }
          } break;
          case FlexAlign::End: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_x = starting_pos_x + remaining_width;
              break;
            case FlexDir::Col:
              align_offset_y = starting_pos_y + remaining_height;
              break;
            }
          } break;
          case FlexAlign::Inherit:
            // unreachable
            break;
          }
        }

        // flex content align (cross axis align all lines)
        if (parent->output.style.flex_wrap == FlexWrap::Wrap) {
          const auto remaining_width = parent->output.get_content_area().fWidth - parent->output.content_size.fWidth;
          const auto remaining_height = parent->output.get_content_area().fHeight - parent->output.content_size.fHeight;

          switch (parent->output.style.flex_content_align) {
          case FlexAlign::Start: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_y = starting_pos_y;
              break;
            case FlexDir::Col:
              align_offset_x = starting_pos_x;
              break;
            }
          } break;
          case FlexAlign::Center: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_y = starting_pos_y + remaining_height / 2.f;
              break;
            case FlexDir::Col:
              align_offset_x = starting_pos_x + remaining_width / 2.f;
              break;
            }
          } break;
          case FlexAlign::End: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              align_offset_y = starting_pos_y + remaining_height;
              break;
            case FlexDir::Col:
              align_offset_x = starting_pos_x + remaining_width;
              break;
            }
          } break;
          case FlexAlign::Inherit:
            // unreachable
            break;
          }
        }

        // flex self align (cross axis align per line)
        auto prev_node = (Node *)nullptr;
        for (const auto &child : line.items) {
          auto self_align_offset_x = 0.f;
          auto self_align_offset_y = 0.f;

          const auto avaliable_width =
            parent->output.style.flex_wrap == FlexWrap::Wrap ? line.width : parent->output.get_content_area().fWidth;
          const auto avaliable_height =
            parent->output.style.flex_wrap == FlexWrap::Wrap ? line.height : parent->output.get_content_area().fHeight;

          const auto remaining_width = avaliable_width - child->output.layout_size.fWidth;
          const auto remaining_height = avaliable_height - child->output.layout_size.fHeight;

          switch (child->output.style.flex_self_align) {
          case FlexAlign::Start: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              self_align_offset_y = parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_y;
              break;
            case FlexDir::Col:
              self_align_offset_x = parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_x;
              break;
            }
          } break;
          case FlexAlign::Center: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              self_align_offset_y =
                (parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_y) + remaining_height / 2.f;
              break;
            case FlexDir::Col:
              self_align_offset_x =
                (parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_x) + remaining_width / 2.f;
              break;
            }
          } break;
          case FlexAlign::End: {
            switch (parent->output.style.flex_dir) {
            case FlexDir::Row:
              self_align_offset_y =
                (parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_y) + remaining_height;
              break;
            case FlexDir::Col:
              self_align_offset_x =
                (parent->output.style.flex_wrap == FlexWrap::Wrap ? 0 : starting_pos_x) + remaining_width;
              break;
            }
          } break;
          case FlexAlign::Inherit:
            // unreachable
            break;
          }

          // apply offset
          child->output.pos.fX += line_offset_x + align_offset_x + self_align_offset_x;
          child->output.pos.fY += line_offset_y + align_offset_y + self_align_offset_y;

          switch (parent->output.style.flex_dir) {
          case FlexDir::Row:
            if (prev_node != nullptr) {
              child->output.pos.fX = prev_node->output.pos.fX + prev_node->output.layout_size.fWidth;
            }
            break;
          case FlexDir::Col:
            if (prev_node != nullptr) {
              child->output.pos.fY = prev_node->output.pos.fY + prev_node->output.layout_size.fHeight;
            }
            break;
          }
          prev_node = child;
        }

        switch (parent->output.style.flex_dir) {
        case FlexDir::Row:
          line_offset_y += line.height;
          break;
        case FlexDir::Col:
          line_offset_x += line.width;
          break;
        }
      }

      // calculate overflow for scrolling
      {
        const auto right = parent->output.pos.fX + parent->output.style.margin_l + parent->output.rect_size.fWidth -
                           parent->output.style.padding_r;
        const auto bottom = parent->output.pos.fY + parent->output.style.margin_t + parent->output.rect_size.fHeight -
                            parent->output.style.padding_b;

        if (!parent->children.empty()) {
          auto content_right = node->output.pos.fX + node->output.layout_size.fWidth;
          auto content_bottom = node->output.pos.fY + node->output.layout_size.fHeight;
          parent->output.content_overflow.fWidth = content_right - right;
          parent->output.content_overflow.fHeight = content_bottom - bottom;
        } else {
          parent->output.content_overflow.fWidth = 0;
          parent->output.content_overflow.fHeight = 0;
        }
      }

      // clamp scroll
      if (parent->output.content_overflow.fHeight > 0) {
        const auto min_y = -parent->output.content_overflow.fHeight;
        parent->style.vscroll_amount = std::clamp(parent->style.vscroll_amount, min_y, 0.f);
      }
    }

    if (node->output.style.display_mode == DisplayMode::Collapsed) {
      return Traverse::SkipChildren;
    }
    return Traverse::Continue;
  });
}

auto Node::layout(SkiaRenderer *renderer) -> void {
  auto reverse_dfs_nodes = layout_pass1();
  layout_pass2(reverse_dfs_nodes, renderer);
  layout_pass3();
  layout_pass4(reverse_dfs_nodes);
  layout_pass5();
}

auto Node::run_mouse_enter_event(int mouse_x, int mouse_y) -> void {
  if (!is_mouse_inside) {
    is_mouse_inside = true;
    if (on_mouse_enter) {
      on_mouse_enter(this, mouse_x, mouse_y);
    }
  }

  if (on_mouse_inside) {
    on_mouse_inside(this, mouse_x, mouse_y);
  }
}

auto Node::run_mouse_leave_event(int mouse_x, int mouse_y) -> void {
  if (is_mouse_inside) {
    is_mouse_inside = false;
    if (on_mouse_leave) {
      on_mouse_leave(this, mouse_x, mouse_y);
    }
  }
}

auto Node::run_mouse_over_event(int mouse_x, int mouse_y) -> void {
  if (!is_mouse_over) {
    is_mouse_over = true;

    {
      auto parent = this->parent;
      while (parent) {
        if (parent->is_mouse_over) {
          parent->is_mouse_over = false;
          if (parent->on_mouse_out) {
            parent->on_mouse_out(parent, mouse_x, mouse_y);
          }
        }
        parent = parent->parent;
      }
    }

    auto propagate = true;
    if (on_mouse_over) {
      propagate = on_mouse_over(this, mouse_x, mouse_y);
    }
    if (propagate) {
      auto parent = this->parent;
      while (parent && propagate) {
        if (!parent->is_mouse_over) {
          parent->is_mouse_over = true;
          if (parent->on_mouse_over) {
            propagate = parent->on_mouse_over(parent, mouse_x, mouse_y);
          }
        }
        parent = parent->parent;
      }
    }
  }
}

auto Node::run_mouse_leave_out_event(int mouse_x, int mouse_y) -> void {
  if (is_mouse_inside) {
    is_mouse_inside = false;
    if (on_mouse_leave) {
      on_mouse_leave(this, mouse_x, mouse_y);
    }
  }

  if (is_mouse_over) {
    is_mouse_over = false;
    if (on_mouse_out) {
      on_mouse_out(this, mouse_x, mouse_y);
    }

    {
      auto parent = this->parent;
      while (parent) {
        if (parent->is_mouse_over) {
          parent->is_mouse_over = false;
          if (parent->on_mouse_out) {
            parent->on_mouse_out(parent, mouse_x, mouse_y);
          }
        }
        parent = parent->parent;
      }
    }
  }
}

auto Node::run_mouse_down_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  if (is_mouse_inside) {
    is_mouse_down = true;

    auto propagate = true;
    if (on_mouse_down) {
      propagate = on_mouse_down(this, button, mouse_x, mouse_y);
    }
    if (propagate) {
      auto parent = this->parent;
      while (parent && propagate) {
        if (parent->on_mouse_down) {
          propagate = parent->on_mouse_down(parent, button, mouse_x, mouse_y);
        }
        parent = parent->parent;
      }
    }
  }
}

auto Node::run_mouse_up_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  // mouse up
  {
    auto propagate = true;
    if (on_mouse_up) {
      propagate = on_mouse_up(this, button, mouse_x, mouse_y);
    }
    if (propagate) {
      auto parent = this->parent;
      while (parent && propagate) {
        if (parent->on_mouse_up) {
          propagate = parent->on_mouse_up(parent, button, mouse_x, mouse_y);
        }
        parent = parent->parent;
      }
    }
  }

  // mouse click
  {
    auto node = this;
    while (node) {
      if (node->is_mouse_down) {
        if (!node->is_mouse_over) {
          node->run_mouse_click_out_event(button, mouse_x, mouse_y);
        } else {
          node->run_mouse_click_in_event(button, mouse_x, mouse_y);
        }
        node->is_mouse_down = false;
        break;
      }
      node = node->parent;
    }
  }

  is_mouse_down = false;
}

auto Node::run_mouse_click_in_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  // mouse click in
  auto propagate = true;
  if (on_mouse_click_in) {
    propagate = on_mouse_click_in(this, button, mouse_x, mouse_y);
  }
  if (propagate) {
    auto parent = this->parent;
    while (parent && propagate) {
      if (parent->on_mouse_click_in) {
        propagate = parent->on_mouse_click_in(parent, button, mouse_x, mouse_y);
      }
      parent = parent->parent;
    }
  }
}

auto Node::run_mouse_click_out_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  // mouse click out
  auto propagate = true;
  if (on_mouse_click_out) {
    propagate = on_mouse_click_out(this, button, mouse_x, mouse_y);
  }
  if (propagate) {
    auto parent = this->parent;
    while (parent && propagate) {
      if (parent->on_mouse_click_out) {
        propagate = parent->on_mouse_click_out(parent, button, mouse_x, mouse_y);
      }
      parent = parent->parent;
    }
  }
}

auto Node::calculate_screen_transform() -> void {
  switch (output.style.transform_mode) {
  case TransformMode::Local: {
    if (parent != nullptr) {
      // calculate scroll
      output.style.transform = style.transform * SkMatrix::I().Translate(SkVector{parent->output.style.hscroll_amount,
                                                                                  parent->output.style.vscroll_amount});

      // calculate screen transform
      output.style.screen_transform = parent->output.style.screen_transform * output.style.transform;
    } else {
      // calculate screen transform
      output.style.screen_transform = output.style.transform;
    }
  } break;
  case TransformMode::Screen: {
    output.style.screen_transform = output.style.transform;
    output.pos = {0, 0};
  } break;
  }
}

auto Node::draw(SkiaRenderer *renderer) -> void {
  const auto canvas = renderer->canvas;

  switch (type) {
  case Type::Rect: {
    auto paint = SkPaint{output.style.color};
    paint.setAntiAlias(true);

    // draw rect
    const auto corners = std::array{SkVector{output.style.border_radius_tl, output.style.border_radius_tl},
                                    SkVector{output.style.border_radius_tr, output.style.border_radius_tr},
                                    SkVector{output.style.border_radius_br, output.style.border_radius_br},
                                    SkVector{output.style.border_radius_bl, output.style.border_radius_bl}};
    const auto rect_pos = output.get_rect_pos();
    const auto rect = SkRect::MakeXYWH(rect_pos.fX, rect_pos.fY, output.rect_size.fWidth, output.rect_size.fHeight);
    auto rrect = SkRRect::MakeEmpty();
    rrect.setRectRadii(rect, corners.data());
    canvas->drawRRect(rrect, paint);

    // draw image
    if (output.style.image != nullptr) {
      const auto image_rect = SkRect::MakeXYWH(rect_pos.fX + output.style.padding_l,               //
                                               rect_pos.fY + output.style.padding_t,               //
                                               output.rect_size.fWidth - output.get_padding_col(), //
                                               output.rect_size.fHeight - output.get_padding_row() //
      );
      canvas->drawImageRect(output.style.image, image_rect, output.style.image_sampling);
    }

    // update clip rect
    output.style.clip_rect = rect;
  } break;
  case Type::Text: {
    auto pos = output.get_rect_pos();
    paragraph->paint(canvas, pos.fX, pos.fY);
  } break;
  }

  // // debug
  // if (renderer->show_debug_lines) {
  //   canvas->save();
  //   {
  //     canvas->setMatrix(output.style.screen_transform);

  //     // draw layout rect
  //     auto paint = SkPaint{SkColors::kDkGray};
  //     paint.setStyle(SkPaint::kStroke_Style);
  //     paint.setStrokeWidth(1);
  //     canvas->drawRect(
  //       SkRect::MakeXYWH(output.pos.fX, output.pos.fY, output.layout_size.fWidth, output.layout_size.fHeight),
  //       paint);

  //     // draw padding rect
  //     paint.setColor(SkColors::kMagenta);
  //     paint.setStyle(SkPaint::kStroke_Style);
  //     paint.setStrokeWidth(1);
  //     auto rect_pos = output.get_rect_pos();
  //     auto padding_row = output.style.padding_l + output.style.padding_r;
  //     auto padding_col = output.style.padding_t + output.style.padding_b;
  //     canvas->drawRect(SkRect::MakeXYWH(rect_pos.fX + output.style.padding_l, rect_pos.fY + output.style.padding_t,
  //                                       output.rect_size.fWidth - padding_row, output.rect_size.fHeight -
  //                                       padding_col),
  //                      paint);
  //   }
  //   canvas->restore();
  // }
}

auto Node::draw_all(SkiaRenderer *renderer) -> void {
  const auto canvas = renderer->canvas;
  const auto original_count = canvas->getSaveCount();

  dfs_with_level([&](Node *node, int level) -> Traverse {
    const auto save_count = canvas->getSaveCount() - original_count;
    if (save_count > level) {
      canvas->restoreToCount(original_count + level);
    }
    canvas->save();

    // set clip
    if (node->parent != nullptr && node->parent->output.style.is_clip_enabled) {
      canvas->setMatrix(node->parent->output.style.screen_transform);
      canvas->clipRect(node->parent->output.style.clip_rect, SkClipOp::kIntersect, false);
    }

    node->calculate_screen_transform();
    canvas->setMatrix(node->output.style.screen_transform);

    if (node->output.style.display_mode != DisplayMode::Shown) {
      return Traverse::SkipChildren;
    }
    node->draw(renderer);
    return Traverse::Continue;
  });

  canvas->restoreToCount(original_count);
}

} // namespace rugui
