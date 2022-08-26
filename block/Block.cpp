#include "Block.hpp"

Block::Block() {
  block_rect.setPosition({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  block_rect.setFillColor(sf::Color::Green);
}

float Block::get_node_size_x(const NODE *node) {
  if (node->type == NODE_TYPE::LABEL) {
    return text_rect_size(node->text).x;
  }

  if (node->type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
    return line_input_field_rect_size().x;
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

  block_rect.setPosition(position);
  block_rect.setSize(block_size);
}

void Block::draw_line_input_attach_field(const sf::Vector2f p_position) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(line_input_field_rect_size());
  r.setFillColor(sf::Color::Red);
  window.draw(r);
}

void Block::Render() {
  // Draw the background rect.
  window.draw(block_rect);

  // Draw text and all other components.
  sf::Vector2f pos = position;
  for (auto child : childrens) {
    if (child.type == NODE_TYPE::LABEL) {
      draw_text(child.text, pos);
      pos.x += text_rect_size(child.text).x;
      pos.x += spacing;
    }

    if (child.type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
      draw_line_input_attach_field(pos);
      pos.x += line_input_field_rect_size().x;
      pos.x += spacing;
    }
  }
}
