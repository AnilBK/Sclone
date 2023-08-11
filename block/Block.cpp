#include "Block.hpp"
#include "../Utils.hpp"

using namespace MATH_UTILITIES;

bool Block::is_mouse_over() { return isMouseOverSprite(block_rect); }

bool Block::is_control_block() { return block_type == BLOCK_TYPES::CONTROL; }

void Block::set_block_type(BLOCK_TYPES p_type) {
  block_type = p_type;
  if (is_control_block()) {
    block_rect.setFillColor(sf::Color::Yellow);
  } else {
    block_rect.setFillColor(sf::Color::Green);
  }
}

void Block::attach_block_next(Block *p_next_block) {
  ERR_CRASH_IF(p_next_block == this, "Children is same as parent.");

  if (p_next_block->is_attached_to_anything) {
    return;
  }

  next_block = p_next_block;
  next_block->is_attached_to_anything = true;
  // Set same position again, so it's child's position can be updated too.
  set_position(position);
}

void Block::set_position(const sf::Vector2f p_pos) {
  position = p_pos;
  block_rect.setPosition(position);

  // If positions aren't regenerated now, then attached blocks lag by a frame.
  // Because _move_attached_blocks() generates positions using old positions.
  // and new position is only used in Render function to update.
  resort_children();

  if (next_block != nullptr) {
    auto snap_rect = _next_block_snap_rect();
    sf::Vector2f next_block_snap_position(snap_rect.getPosition());

    next_block->set_position(next_block_snap_position);
  }
}

Block::Block() {
  if (DEBUG_BLOCK_SPAWN_STATS) {
    static int c_count = 0;
    std::cout << "[" << c_count << "] Created object: " << function_identifier
              << "\n";
    c_count++;
  }

  set_position({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  block_rect.setOutlineThickness(2.0f);
  set_block_type(BLOCK_TYPES::INSTRUCTION);
}

Block::~Block() {
  if (DEBUG_BLOCK_SPAWN_STATS) {
    static int d_count = 0;
    std::cout << "[" << d_count << "] Destroyed object: " << function_identifier
              << "\n";
    d_count++;
  }
}

sf::FloatRect Block::_previous_block_snap_rect() {
  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() - sf::Vector2f(0.0f, snap_rect_size.y);
  return {snap_rect_position, snap_rect_size};
}

sf::FloatRect Block::_next_block_snap_rect() {
  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() +
      sf::Vector2f(0.0f, block_without_attached_block_size.height);
  return {snap_rect_position, snap_rect_size};
}

void Block::show_previous_block_snap_hint() {
  auto r = _previous_block_snap_rect();
  auto r_pos = r.getPosition();
  auto r_size = r.getSize();

  sf::RectangleShape previous_block_snap_hint;
  previous_block_snap_hint.setPosition(r_pos);
  previous_block_snap_hint.setSize(r_size);
  previous_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(previous_block_snap_hint);
}

void Block::show_next_block_snap_hint() {
  auto r = _next_block_snap_rect();
  auto r_pos = r.getPosition();
  auto r_size = r.getSize();

  sf::RectangleShape next_block_snap_hint;
  next_block_snap_hint.setPosition(r_pos);
  next_block_snap_hint.setSize(r_size);
  next_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(next_block_snap_hint);
}

bool Block::can_mouse_snap_to_top() {
  // if (previous != nullptr) {
  // return false;
  // }
  if (is_control_block()) {
    return false;
  }

  return isMouseOverRect(_previous_block_snap_rect());
}

bool Block::can_mouse_snap_to_bottom() {
  if (next_block != nullptr) {
    return false;
  }

  return isMouseOverRect(_next_block_snap_rect());
}

std::optional<BlockAttachNode *> Block::get_node_to_attach_block_inside() {
  auto nodes = get_block_attach_nodes(false);
  for (auto &node : nodes) {
    if (node->can_snap_block_inside()) {
      // TODO :: Check if the snap place is already occupied.
      return node;
    }
  }

  return {};
}

void Block::show_possible_snap_hints() {
  if (can_mouse_snap_to_top()) {
    show_previous_block_snap_hint();
  }

  if (can_mouse_snap_to_bottom()) {
    show_next_block_snap_hint();
  }

  auto attach_block_inside_node = get_node_to_attach_block_inside();
  if (attach_block_inside_node.has_value()) {
    attach_block_inside_node.value()->_show_snap_for_attachable_block();
  }
}

sf::FloatRect Block::full_rect() {
  // Top most block's rect.
  sf::FloatRect rect = block_rect.getGlobalBounds();

  if (can_block_attach_inside) {
    // These are 'BlockAttachNodes'.
    // They occupy some space too.
    // Blocks without any blocks attached to them also have some rect_size().
    auto nodes = get_block_attach_nodes(false);

    for (auto &node : nodes) {
      sf::FloatRect l_shape_rect = node->full_rect();
      rect = merge_rects(rect, l_shape_rect);
    }
  }

  block_without_attached_block_size = rect;

  // Attached Block's rect.
  if (next_block != nullptr) {
    rect = merge_rects(rect, next_block->full_rect());
  }

  return rect;
}

void Block::update_children_sizes() {
  // TODO: Do other kinds of nodes require _update() ??

  if (can_block_attach_inside) {
    // As of now, only BlockAttachNode's require their internal sizes to be
    // updated.
    auto nodes = get_block_attach_nodes();

    for (auto &node : nodes) {
      node->_update_internal_sizes();
    }
  }
}

void Block::_resort_children_single_liner() {
  // Most blocks are single lines. For such blocks, we need not perform
  // all the if-checks and lots of additional code doesn't need to be
  // performed.
  // resort_children_multiple() handles this case as well.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  float max_height = 0.0f;

  for (auto &child : childrens) {
    child->set_position(pos);
    auto child_size = child->rect_size();
    pos.x += child_size.x + spacing;
    max_height = std::max(max_height, child_size.y);
  }

  block_rect.setSize({pos.x + padding_right - position.x,
                      max_height + padding_up + padding_down});
}

void Block::_resort_children_multiple() {
  // If there are children with multiple linebreaks, then we have to care about
  // about alignment of different children. This dirty functions tries to handle
  // all those edge-cases.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);

  float max_height = 0.0f;

  bool first_line_break_encountered = false;

  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      if (!first_line_break_encountered) {
        // First row has completed.
        // The base size is equal to the size of all the childrens till now.
        block_rect.setSize({pos.x + padding_right - position.x,
                            max_height + padding_up + padding_down});
        // 'padding_up' is already added earlier, so no need to add that again.
        // 2.0 is outline thickness.
        pos.y += padding_down + 2.0;
        first_line_break_encountered = true;
      }

      // Reset to new line.
      pos.x = position.x;
      pos.y += max_height;
    }

    child->set_position(pos);

    auto child_size = child->rect_size();

    pos.x += child_size.x + spacing;
    max_height = std::max(max_height, child_size.y);

    if (child->type == BLOCK_ATTACH_NODE) {
      pos.x = position.x;
      pos.y += child_size.y;
      // The horizontal line on the bottom the 'BlockAttachNode' is 30px.
      auto casted = dynamic_cast<BlockAttachNode *>(child.get());
      if (casted && casted->draw_bottom_part) {
        // This is to handle a special case, because we want to draw the
        // remaining children on the bottom of the L shape of the current
        // 'BlockAttachNode'.
        pos.y -= 30;
      }

      max_height = 30; // + 2.0F
    }
  }

  if (!first_line_break_encountered) {
    // That means this is a single liner block & we haven't set the base
    // position yet.
    block_rect.setSize({pos.x + padding_right - position.x,
                        max_height + padding_up + padding_down});
  }
}

void Block::resort_children() {
  if (can_block_attach_inside) {
    update_children_sizes(); // As of now, this is called only for block which
                             // has a 'BlockAttachNode'.
    _resort_children_multiple();
  } else {
    _resort_children_single_liner();
  }

  auto rect = full_rect();
  block_full_size = rect.getSize();
}

void Block::render_full_rect() {
  auto rect = full_rect();
  sf::Vector2f size = rect.getSize();

  sf::RectangleShape debug_block_rect_full;
  debug_block_rect_full.setPosition(position);
  debug_block_rect_full.setSize(size);
  debug_block_rect_full.setFillColor(sf::Color(255, 0, 0, 200));
  window.draw(debug_block_rect_full);
}

void Block::render_base() { window.draw(block_rect); }

void Block::render_children() {
  for (const auto &child : childrens) {
    child->Render();
  }

  constexpr bool visualize_block = false;
  if (visualize_block) {
    render_full_rect();
  }
}

void Block::Render() {
  resort_children();
  render_base();
  render_children();
}

bool Block::_process_left_click_on_children(sf::Event event) {
  // First process child inputs separately.
  for (const auto &child : childrens) {
    if (child->is_mouse_over()) {
      if (child->left_click_action()) {
        return true;
      }
    }
  }

  return false;
}

bool Block::is_interacted() {
  if (is_mouse_over()) {
    return true;
  }

  if (any_node_already_pressed()) {
    return true;
  }

  return false;
}

bool Block::any_node_already_pressed() {
  for (const auto &child : childrens) {
    if (child->pressed) {
      return true;
    }
  }
  return false;
}

void Block::deselect_all_nodes() {
  for (auto &child : childrens) {
    if (child->pressed) {
      child->deselect_node();
    }
  }
}

void Block::background_inputs(sf::Event event) {
  _update_mouse_pick_nodes();

  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Right) {
      deselect_all_nodes();

      if (dragging) {
        dragging = false;
      }
    }
  }
}

void Block::_update_mouse_pick_nodes() {
  if (!has_mouse_pick_node) {
    return;
  }

  auto editor_mouse_pos =
      static_cast<sf::Vector2i>(get_mouse_position_wrt_2d_editor());
  auto m_x_str = std::to_string(editor_mouse_pos.x);
  auto m_y_str = std::to_string(editor_mouse_pos.y);

  for (auto &child : childrens) {
    auto pick_button = dynamic_cast<PickWithMouseNode *>(child.get());
    if (pick_button) {
      if (pick_button->is_pressed()) {
        auto x_index = pick_button->childs_index_for_x;
        auto y_index = pick_button->childs_index_for_y;

        if (x_index != -1) {
          childrens[x_index]->set_text(m_x_str);
        }

        if (y_index != -1) {
          childrens[y_index]->set_text(m_y_str);
        }
      }
    }
  }
}

bool Block::left_clicked(sf::Event event) {
  // Already dragging, so undrag it.
  if (dragging) {
    deselect_all_nodes();
    dragging = false;
    // Left click was consumed.
    return true;
  }

  // Not already dragging and mouse isn't over.
  if (!is_mouse_over()) {
    return false;
  }

  // Is mouse over and one of the children is clicked.
  if (_process_left_click_on_children(event)) {
    return true;
  } else {
    // It's children weren't clicked.
    // Select/Drag the parent block instead.
    dragging = true;
    return true;
  }

  return false;
}

void Block::handle_inputs(sf::Event event) {
  auto m_pos = get_mouse_position();

  // Update mouse position cache, as all the child components use the cached
  // position. Getting the position for every UI Nodes wouldn't be good.
  mouse_position = static_cast<sf::Vector2i>(m_pos);

  // For some reason, if we do this check on Render()
  // then the value resets after we press right click.
  // Weird.
  if (dragging) {
    set_position(m_pos);
  }

  for (auto &child : childrens) {
    child->handle_inputs(event);
  }

  _update_mouse_pick_nodes();
}

std::vector<BlockAttachNode *>
Block::get_block_attach_nodes(bool with_nodes_attached) {
  std::vector<BlockAttachNode *> attached_blocks_vec;

  for (auto &child : childrens) {
    if (child->type != BLOCK_ATTACH_NODE) {
      continue;
    }

    auto child_casted = dynamic_cast<BlockAttachNode *>(child.get());

    if (child_casted) {
      auto attached_block = child_casted->attached_block;

      if (with_nodes_attached) {
        if (attached_block != nullptr) {
          attached_blocks_vec.push_back(child_casted);
        }
      } else {
        attached_blocks_vec.push_back(child_casted);
      }
    }
  }

  return attached_blocks_vec;
}

std::optional<std::string>
Block::get_bound_value(const std::string &query) const {
  for (const auto &child : childrens) {
    if (child->bind_string == query) {
      return child->get_text();
    }
  }

  auto unbound_msg_str = "[Debug] String \"" + query + "\" Possibly Unbound.";
  ERR_CRASH_IF(true, unbound_msg_str);

  return {};
}

float Block::get_bound_value_or(const std::string &query,
                                float default_value) const {
  for (const auto &child : childrens) {
    if (child->bind_string == query) {
      return MATH_UTILITIES::str_to_float(child->get_text(), default_value);
    }
  }

  // Not found.
  return default_value;
}

std::string Block::get_code() {
  std::string code;

  if (output_code_callback) {
    code += output_code_callback(*this);
  }

  if (can_block_attach_inside) {
    code += "{\n";
  }

  auto nodes = get_block_attach_nodes();

  if (!nodes.empty()) {
    size_t n = nodes.size();
    size_t i = 0;

    for (auto &node : nodes) {
      code += node->attached_block->get_code();

      if (i < n - 1) {
        code += "}else{";
      }

      i++;
    }
  }

  if (can_block_attach_inside) {
    code += "\n}\n";
  }

  // We could make this a separate callback like 'output_code_callback', but
  // there's no need to add an additional member to the block class, as most
  // blocks don't require that.
  if (function_identifier == "block_alternate_between") {
    // Toggle the internal state of flip flop node.
    // See code_alternate_between_init() in 'OutputCodeCallbacks.hpp'.
    code += "##SPRITE_NAME##__flip_flop = !##SPRITE_NAME##__flip_flop;";
  }

  if (next_block != nullptr) {
    code += next_block->get_code();
  }

  // This applies to code everywhere, so we perform the replace
  //  operation here.
  _apply_editor_shortcut_macros(code);

  return code;
}

void Block::_apply_editor_shortcut_macros(std::string &code) {
  // EDITOR SHORCUT MACROS:
  //  E.g: Writing #POS# in any input fields of the editor -> gets position
  //  of the current sprite.
  using namespace STRING_UTILITIES;
  replaceAll(code, "#POS#", "##SPRITE_NAME##.getPosition()");
  replaceAll(code, "#WIN_W#", "width");
  replaceAll(code, "#WIN_H#", "height");
  replaceAll(code, "#M_X#", "sf::Mouse::getPosition(window).x");
  replaceAll(code, "#M_Y#", "sf::Mouse::getPosition(window).y");
}

std::string Block::get_code_for_init() {
  // Very few blocks require initialization code.
  // So, generate code only if there is some code callbacks for initialization.
  // The generated code from here goes to ' //###INIT_CODES### ' section of
  // template.cpp.

  if (!output_code_for_init_callback) {
    return "";
  }

  std::string code = output_code_for_init_callback(*this);

  _apply_editor_shortcut_macros(code);

  return code;
}
