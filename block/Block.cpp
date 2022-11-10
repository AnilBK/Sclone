#include "Block.hpp"

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
  ERR_FAIL_COND_CRASH(p_next_block == this, "Children is same as parent.");

  next_block = p_next_block;
  set_position(position);
  // Set same position again, so it's child's position can be updated too.
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
    sf::Vector2f next_block_snap_position(snap_rect.left, snap_rect.top);

    next_block->set_position(next_block_snap_position);
  }
}

Block::Block() {
  static int c_count = 0;
  std::cout << "[" << c_count << "] Created object: " << function_identifier
            << "\n";
  c_count++;

  set_position({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  block_rect.setOutlineThickness(2.0f);
  set_block_type(BLOCK_TYPES::INSTRUCTION);
}

Block::~Block() {
  static int d_count = 0;
  std::cout << "[" << d_count << "] Destroyed object: " << function_identifier
            << "\n";
  d_count++;
}

sf::FloatRect Block::_previous_block_snap_rect() {
  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() - sf::Vector2f(0.0f, snap_rect_size.y);
  return {snap_rect_position, snap_rect_size};
}

sf::FloatRect Block::_next_block_snap_rect() {
  sf::Vector2f pos{0.0f, STARTING_BLOCK_SIZE.y};

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Reset it's x to create a block on the next line.
      pos.y += child->rect_size().y + 45; // STARTING_BLOCK_SIZE.y;
    }
  }

  // Margins
  // Account for the block position decreased during padding.
  pos.y += padding_up + padding_down;

  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() + sf::Vector2f(0.0f, pos.y);
  return {snap_rect_position, snap_rect_size};
}

void Block::show_previous_block_snap_hint() {
  auto r = _previous_block_snap_rect();
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width, r.height);

  sf::RectangleShape previous_block_snap_hint;
  previous_block_snap_hint.setPosition(r_pos);
  previous_block_snap_hint.setSize(r_size);

  previous_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(previous_block_snap_hint);
}

void Block::show_next_block_snap_hint() {
  auto r = _next_block_snap_rect();
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width, r.height);

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

  return _previous_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

bool Block::can_mouse_snap_to_bottom() {
  if (next_block != nullptr) {
    return false;
  }
  return _next_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

void Block::process_inside_snap_hints(bool attach_block_requested,
                                      Block *current_dragging_block_ref) {
  auto nodes = get_block_attach_nodes(false);
  for (auto &node : nodes) {
    if (!node->can_snap_block_inside()) {
      continue;
    }

    node->_show_snap_for_attachable_block();

    if (attach_block_requested) {
      // TODO :: Check if the snap place is already occupied.
      // TODO :: current dragging block ref should be pushed back where the
      // snap hint is showing.
      node->attached_block = current_dragging_block_ref;
      node->attached_block->dragging = false;
      current_dragging_block_ref->dragging = false;

      set_position(position); // Refresh for the newly added block.
      return;
    }
  }
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

void Block::_recalculate_rect() {
  sf::Vector2f pos = position;
  sf::FloatRect merged_rect(pos, STARTING_BLOCK_SIZE);

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Reset it's x to create a block on the next line.
      pos.x = position.x + 15.0f;
      pos.y += merged_rect.height; // STARTING_BLOCK_SIZE.y;
      continue;
    }

    sf::FloatRect current_rect(pos, child->rect_size());
    merged_rect = merge_rects(merged_rect, current_rect);

    pos.x += child->rect_size().x;
    pos.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  merged_rect.width += padding_left + padding_right;
  merged_rect.height += padding_up + padding_down;

  block_rect.setSize({merged_rect.width, merged_rect.height});

  // The next block snap rect is at the bottomm of the block which effectively
  // gives the height of the current block.
  auto next_block_snap_rect = _next_block_snap_rect();
  auto full_size_x = next_block_snap_rect.width;
  auto full_size_y = next_block_snap_rect.top - block_rect.getPosition().y;

  block_full_size = {full_size_x, full_size_y};
}

void Block::RenderRectsBackground() {
  sf::RectangleShape block_bg;
  block_bg.setOutlineThickness(2.0f);
  block_bg.setPosition(position);
  block_bg.setFillColor(block_rect.getFillColor());

  sf::FloatRect merged_rect(position, STARTING_BLOCK_SIZE);

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Render everything till now.
      block_bg.setSize({merged_rect.width, merged_rect.height});
      window.draw(block_bg);
      return;
    }

    sf::FloatRect current_rect(child->_pos, child->rect_size());
    merged_rect = merge_rects(merged_rect, current_rect);
  }

  // Margins
  // Account for the block position decreased during padding.
  merged_rect.width += padding_left + padding_right;
  merged_rect.height += padding_up; // + padding_down;

  block_bg.setSize({merged_rect.width, merged_rect.height});
  window.draw(block_bg);
}

sf::FloatRect Block::full_rect() {
  // Top most block's rect.
  sf::FloatRect rect = block_rect.getGlobalBounds();

  // Attached Block's rect.
  if (next_block != nullptr) {
    rect = merge_rects(rect, next_block->full_rect());
  }

  if (!can_block_snap_inside) {
    return rect;
  }

  // These are 'BlockAttachNodes'.
  // They occupy some space too.
  // Blocks without any blocks attached to them also have some rect_size().
  auto nodes = get_block_attach_nodes(false);

  for (auto &node : nodes) {
    sf::FloatRect l_shape_rect = node->rect_size_with_outlines();
    //+  sf::Vector2f(0.0f, 45.0f));

    if (auto attached_block = node->attached_block; attached_block != nullptr) {
      l_shape_rect = merge_rects(l_shape_rect, attached_block->full_rect());
    }

    rect = merge_rects(rect, l_shape_rect);
  }

  return rect;
}

void Block::update_children_sizes() {
  // TODO?? Do other kinds of nodes require _update() ??
  //  Block_Attach_Node's require their internal sizes to be updated.
  auto nodes = get_block_attach_nodes();

  for (auto &node : nodes) {
    node->_update_internal_sizes();
  }
}

void Block::resort_children() {
  update_children_sizes();

  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);

  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      pos.x = position.x;
      pos.y += STARTING_BLOCK_SIZE.y;
    }

    child->_pos = pos;
    pos.x += child->rect_size().x + spacing;

    if (child->type == BLOCK_ATTACH_NODE) {
      pos.x = position.x;
      pos.y += child->rect_size().y;
    }
  }
}

void Block::RenderFullRect() {
  auto rect = full_rect();
  sf::Vector2f size = {rect.width, rect.height};

  sf::RectangleShape debug_block_rect_full;
  debug_block_rect_full.setPosition(position);
  debug_block_rect_full.setSize(size);

  debug_block_rect_full.setFillColor(sf::Color(255, 0, 0, 200));
  window.draw(debug_block_rect_full);
}

void Block::render_children() {
  for (const auto &child : childrens) {
    child->Render(child->_pos);
  }

  constexpr bool visualize_attached_blocks = false;
  if (!visualize_attached_blocks) {
    return;
  }

  const auto nodes = get_block_attach_nodes();
  for (auto &node : nodes) {
    node->attached_block->RenderFullRect();
  }
}

// Draw text and all other components.
void Block::RenderComponents() {
  resort_children();
  render_children();
}

void Block::Render() {
  // Draw the background rect.
  // window.draw(block_rect);
  RenderRectsBackground();
  RenderComponents();
}

bool Block::_process_left_click_on_children(sf::Event event) {
  // Returns true if any of the child performed 'press' action.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  // First process child inputs separately.
  for (const auto &child : childrens) {
    if (child->is_mouse_over(pos)) {
      if (child->left_click_action()) {
        return true;
      }
    }

    pos.x += child->rect_size().x;
    pos.x += spacing;
  }

  return false;
}

bool Block::_any_node_already_pressed() {
  for (const auto &child : childrens) {
    if (child->pressed) {
      return true;
    }
  }
  return false;
}

void Block::_deselect_all_nodes() {
  for (auto &child : childrens) {
    if (child->pressed) {
      child->pressed = false;
    }
  }
}

void Block::_process_events(sf::Event event) {
  // For some reason, if we do this check on Render()
  // then the value resets after we press right click.
  // Weird.
  if (dragging) {
    set_position((sf::Vector2f)mouse_position);
  }

  bool rect_dirty = false;
  bool any_line_inputs_pressed = false;

  for (auto &child : childrens) {
    if (child->type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD ||
        child->type == NODE_TYPE::DROP_DOWN) {
      child->_process_event(event);
      if (child->pressed) {
        any_line_inputs_pressed = true;
      }
    }

    if (child->type != NODE_TYPE::BUTTON) {
      continue;
    }

    if (child->get_text() != "Pick^") {
      continue;
    }

    if (child->pressed) {
      childrens[2]->set_text(std::to_string(mouse_position.x));
      childrens[4]->set_text(std::to_string(mouse_position.y));
      rect_dirty = true;
    }
  }

  // BUG: Typing on a LineInput Node should trigger recalculation of rect.
  // Workaround: We always _recalculate rect like below
  // Also, if LineInput Node isn't selected then there's no change i.e it's
  // rect can change. any_line_inputs_pressed -> workaround variable.
  if (rect_dirty || any_line_inputs_pressed) {
    _recalculate_rect();
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    // Left to drag, right to undrag.
    // We may use left to undrag as well, but those clicks occur so
    // fast, mostly it causes toggle on/off/on.. conditions.
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (_any_node_already_pressed()) {
        _deselect_all_nodes();
      }

      if (_process_left_click_on_children(event)) {
        return;
      }

      // No any internal blocks were clicked.
      if (!dragging && is_mouse_over()) {
        dragging = true;
      }

    } else if (event.mouseButton.button == sf::Mouse::Right) {
      // TODO: All clicks outside a block should invalidate pressed state of
      // any NODE.
      _deselect_all_nodes();

      if (dragging) {
        dragging = false;
      }
    }
  }
}

std::optional<std::string>
Block::get_bound_value(const std::string &query) const {
  for (const auto &child : childrens) {
    if (child->bind_string == query) {
      return child->get_text();
    }
  }

  auto unbound_msg_str = "[Debug] String \"" + query + "\" Possibly Unbound.";
  ERR_FAIL_COND_CRASH(true, unbound_msg_str);

  return {};
}

std::string Block::get_code() {
  std::string code;

  if (output_code_callback) {
    code += output_code_callback(*this);
  }

  if (can_block_snap_inside) {
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

  if (can_block_snap_inside) {
    code += "\n}\n";
  }

  if (next_block != nullptr) {
    code += next_block->get_code();
  }

  // EDITOR SHORCUT MACROS:
  //  Shortcut in the editor to get position of the current sprite.
  //  This applies to code everywhere, so we perform the replace operation here.
  replaceAll(code, "#POS#", "##SPRITE_NAME##.getPosition()");
  replaceAll(code, "#WIN_W#", "width");
  replaceAll(code, "#WIN_H#", "height");
  replaceAll(code, "#M_X#", "sf::Mouse::getPosition(window).x");
  replaceAll(code, "#M_Y#", "sf::Mouse::getPosition(window).y");

  return code;
}
