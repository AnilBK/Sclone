#include "Block.hpp"

void Block::set_position(const sf::Vector2f pos) {
  position = pos;
  block_rect.setPosition(position);

  if (block_inside != nullptr) {
    sf::Vector2f inside_block_snap_position(
        _inside_block_snap_rect().left + 15.0f,
        _inside_block_snap_rect().top + padding_up);
    block_inside->set_position(inside_block_snap_position);
  }

  if (next_block != nullptr) {
    sf::Vector2f next_block_snap_position(_next_block_snap_rect().left,
                                          _next_block_snap_rect().top);
    next_block->set_position(next_block_snap_position);
  }
}

Block::Block() {
  set_position({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  set_block_type(BLOCK_TYPES::INSTRUCTION);
}

void Block::_recalculate_rect() {
  /*
  // sf::Vector2f block_position{position.x - padding_left,
  //                             position.y - padding_up};
  sf::Vector2f block_size = STARTING_BLOCK_SIZE;

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      block_size.y += child->rect_size().y / 2.0f;
      // Reset it's x to create a block on the next line.
      //  block_size.y += child->rect_size().y; should be this
      //      pos.y += 45.0f;
      continue;
    }
    // instead of adding merge two rects. because not all blocks are continous
    // in a same row.
    block_size.x += child->rect_size().x;
    block_size.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  block_size.x += padding_left + padding_right;
  block_size.y += padding_up + padding_down;

  block_rect.setSize(block_size);
  */

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
}

bool Block::is_control_block() {
  return block_type == BLOCK_TYPES::CONTROL;
  // if (childrens.at(0)->get_text() == "When Program Starts") {
  // return true;
  // }

  // return false;
}

sf::FloatRect Block::_previous_block_snap_rect() {
  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() - sf::Vector2f(0.0f, snap_rect_size.y);
  return {snap_rect_position, snap_rect_size};
}

sf::FloatRect Block::_inside_block_snap_rect() {
  auto snap_rect_size =
      sf::Vector2f(block_rect.getSize().x, 10 + padding_up + padding_down);
  auto snap_rect_position =
      block_rect.getPosition() + sf::Vector2f(0, STARTING_BLOCK_SIZE.y);
  return {snap_rect_position, snap_rect_size};
}

sf::FloatRect Block::_next_block_snap_rect() {

  sf::Vector2f pos{0.0f, STARTING_BLOCK_SIZE.y};

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Reset it's x to create a block on the next line.
      pos.y += 90 + 45; // STARTING_BLOCK_SIZE.y;
      continue;
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

void Block::show_inside_block_snap_hint() {
  auto r = _inside_block_snap_rect();
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width, r.height);

  sf::RectangleShape next_block_snap_hint;
  next_block_snap_hint.setPosition(r_pos);
  next_block_snap_hint.setSize(r_size);

  next_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(next_block_snap_hint);
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

bool Block::can_mouse_snap_to_inside() {
  if (!can_block_snap_inside) {
    return false;
  }

  if (block_inside != nullptr) {
    return false;
  }

  return _inside_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

bool Block::can_mouse_snap_to_bottom() {
  if (next_block != nullptr) {
    return false;
  }
  return _next_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

void Block::attach_block_next(Block *p_next_block) {
  if (p_next_block == this) {
    ERR_FAIL_COND_CRASH(false, "Children is same as parent.");
  }

  next_block = p_next_block;
  set_position(position);
  // Set same position again, so it's child's position can be updated too.
}

void Block::attach_block_inside(Block *p_inside_block) {
  if (p_inside_block == this) {
    ERR_FAIL_COND_CRASH(false, "Children is same as parent.");
  }

  block_inside = p_inside_block;
  set_position(position);
  // Set same position again, so it's child's position can be updated too.
}

std::string Block::get_code() {

  std::string code;

  if (output_code_callback) {
    code += output_code_callback(*this);
  }

  if (can_block_snap_inside) {
    code += "{\n";
  }

  if (block_inside != nullptr) {
    code += block_inside->get_code();
  }

  if (can_block_snap_inside) {
    code += "}\n";
  }

  if (next_block != nullptr) {
    code += next_block->get_code();
  }

  return code;
}

void Block::Render() {
  // Draw the background rect.
  window.draw(block_rect);

  // Draw text and all other components.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Make this changes in get_rect() as well.
      pos.x = position.x; // Reset it's x to create a block on the next line.
      pos.y += 45.0f;
      child->Render(pos);
      pos.x += 15;
      continue;
    }

    child->Render(pos);
    pos.x += child->rect_size().x;
    pos.x += spacing;
  }
}

std::optional<std::string> Block::get_bound_value(const std::string &query) {
  for (const auto &child : childrens) {
    if (child->bind_string == query) {
      return child->get_text();
    }
  }

  auto unbound_msg_str = "[Debug] String \"" + query + "\" Possibly Unbound.";
  ERR_FAIL_COND_CRASH(false, unbound_msg_str);

  return {};
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
  for (auto &child : childrens) {
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
    if (child->type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
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
  // Also, if LineInput Node isn't selected then there's no change i.e it's rect
  // can change.
  // any_line_inputs_pressed -> workaround variable.
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
      if (!dragging && isMouseOverSprite(block_rect)) {
        dragging = true;
      }

      /*
            // Process Node's callbacks, if any;
            bool callback_called = false;
            for (auto &child : childrens) {
              if (child.type == NODE_TYPE::BUTTON && child.text == "Pick^") {
                if (!child.pressed) {
                  //  child.pressed = true;
                  callback_called = true;
                }
                //          callback_called = true;
              }
              // TODO: Check if mouse is over the sprite.
              //  if (child.callback) {
              //   child.callback();
              //  callback_called = true;
              //}
            }

            if (callback_called) {
              return;
            }
      */
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
