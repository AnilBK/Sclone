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

float Block::get_node_size_x(const NODE *node) {
  if (node->type == NODE_TYPE::LABEL) {
    return text_rect_size(node->text).x;
  } else if (node->type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
    if (node->text != "") {
      return std::max(text_rect_size(node->text).x,
                      line_input_field_rect_size().x);
    }
    return line_input_field_rect_size().x;
  } else if (node->type == NODE_TYPE::BUTTON) {
    sf::Vector2f text_box_size{text_rect_size(node->text)};
    sf::Vector2f btn_size{std::max(text_box_size.x, button_rect_size().x) + 5,
                          std::max(text_box_size.y, button_rect_size().y)};
    return btn_size.x;
  }

  return 0.0f;
}

void Block::_recalculate_rect() {
  // sf::Vector2f block_position{position.x - padding_left,
  //                             position.y - padding_up};
  sf::Vector2f block_size = STARTING_BLOCK_SIZE;

  for (auto child : childrens) {
    block_size.x += get_node_size_x(&child);
    block_size.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  block_size.x += padding_left + padding_right;
  block_size.y += padding_up + padding_down;

  block_rect.setSize(block_size);
}

void Block::draw_line_input_attach_field(const sf::Vector2f p_position) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(line_input_field_rect_size());
  r.setFillColor(sf::Color::Red);
  window.draw(r);
}

void Block::draw_button(const sf::Vector2f p_position,
                        const sf::Vector2f p_size) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(p_size);
  r.setFillColor(sf::Color(192, 195, 198, 255));
  window.draw(r);
}

void Block::Render() {
  // Draw the background rect.
  window.draw(block_rect);

  // Draw text and all other components.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  for (auto child : childrens) {
    if (child.type == NODE_TYPE::LABEL) {
      draw_text(child.text, pos);
      pos.x += text_rect_size(child.text).x;
      pos.x += spacing;
    } else if (child.type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
      draw_line_input_attach_field(pos);
      if (child.text != "") {
        // Usually empty for line input attach field so.
        draw_text(child.text, pos);
        pos.x += std::max(text_rect_size(child.text).x,
                          line_input_field_rect_size().x);
      } else {
        pos.x += line_input_field_rect_size().x;
      }
      pos.x += spacing;
    } else if (child.type == NODE_TYPE::BUTTON) {
      sf::Vector2f text_box_size{text_rect_size(child.text)};
      sf::Vector2f btn_size{std::max(text_box_size.x, button_rect_size().x) + 5,
                            std::max(text_box_size.y, button_rect_size().y)};
      draw_button(pos, btn_size);
      draw_text(child.text, pos);
      pos.x += btn_size.x;
      pos.x += spacing;
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
    if (child.type == NODE_TYPE::BUTTON && child.text == "Pick^" &&
        child.pressed) {
      childrens[2].text = std::to_string(mouse_position.x) + " ";
      childrens[4].text = std::to_string(mouse_position.y) + " ";
    }
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    // Left to drag, right to undrag.
    // We may use left to undrag as wll, but those clicks occur so fast,
    // mostly it causes toggle on/off/on.. conditions.
    if (event.mouseButton.button == sf::Mouse::Left) {
      // Process Node's callbacks, if any;
      bool callback_called = false;
      for (auto &child : childrens) {
        if (child.type == NODE_TYPE::BUTTON && child.text == "Pick^") {
          if (!child.pressed) {
            child.pressed = true;
          }
          callback_called = true;
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

      if (!dragging && isMouseOverSprite(block_rect)) {
        dragging = true;
      }
    } else if (event.mouseButton.button == sf::Mouse::Right) {

      for (auto &child : childrens) {
        if (child.type == NODE_TYPE::BUTTON && child.text == "Pick^") {
          if (child.pressed) {
            child.pressed = false;
          }
        }
        // TODO: Check if mouse is over the sprite.
        //  if (child.callback) {
        //   child.callback();
        //  callback_called = true;
        //}
      }

      if (dragging) {
        dragging = false;
      }
    }
  }
}
