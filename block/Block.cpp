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

  for (auto child : childrens) {
    block_size.x += child.get_rect_size_x();
    block_size.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  block_size.x += padding_left + padding_right;
  block_size.y += padding_up + padding_down;

  block_rect.setSize(block_size);
}

void Block::Render() {
  // Draw the background rect.
  window.draw(block_rect);

  // Draw text and all other components.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  for (auto child : childrens) {
    child.Render(pos);
    pos.x += child.get_rect_size_x();
    pos.x += spacing;
  }
}

bool Block::_process_left_click_on_children() {
  // Returns true if any of the child was pressed.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  // First process child inputs separately.
  for (auto &child : childrens) {
    if (child.is_mouse_over(pos)) {
      if (child.left_click_action()) {
        return true;
      }
    }

    pos.x += child.get_rect_size_x();
    pos.x += spacing;
  }

  return false;
}

bool Block::_any_node_already_pressed() {
  return std::any_of(childrens.begin(), childrens.end(),
                     [](NODE n) { return n.pressed; });
}

void Block::_deselect_all_nodes() {
  for (auto &child : childrens) {
    if (child.pressed) {
      child.pressed = false;
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

  for (auto &child : childrens) {
    if (child.is_mouse_picker_button() && child.pressed) {
      childrens[2].text = std::to_string(mouse_position.x) + " ";
      childrens[4].text = std::to_string(mouse_position.y) + " ";
    }
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    // Left to drag, right to undrag.
    // We may use left to undrag as well, but those clicks occur so
    // fast, mostly it causes toggle on/off/on.. conditions.
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (_any_node_already_pressed()) {
        _deselect_all_nodes();
      }

      if (_process_left_click_on_children()) {
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
      // TODO: All clicks outside a block should invalidate pressed state of any
      // NODE.
      _deselect_all_nodes();

      if (dragging) {
        dragging = false;
      }
    }
  }
}
