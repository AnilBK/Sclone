#include "Block.hpp"

void Block::set_position(const sf::Vector2f pos) {
  position = pos;
  block_rect.setPosition(position);
}

Block::Block() {
  set_position({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  block_rect.setFillColor(sf::Color::Green);
}

void Block::_recalculate_rect() {
  // sf::Vector2f block_position{position.x - padding_left,
  //                             position.y - padding_up};
  sf::Vector2f block_size = STARTING_BLOCK_SIZE;

  for (const auto &child : childrens) {
    block_size.x += child->rect_size().x;
    block_size.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  block_size.x += padding_left + padding_right;
  block_size.y += padding_up + padding_down;

  block_rect.setSize(block_size);
}

bool Block::is_control_block() {
  if (childrens.at(0)->get_text() == "When Program Starts") {
    return true;
  }

  return false;
}

std::string Block::get_code() {

  std::string code;

  if (output_code_callback) {
    code += output_code_callback(*this);
  }

  return code;
}

void Block::Render() {
  // Draw the background rect.
  window.draw(block_rect);

  // Draw text and all other components.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  for (const auto &child : childrens) {
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
