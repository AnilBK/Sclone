#include "Block.hpp"

void Block::draw_line_input_attach_field(const sf::Vector2f p_position) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(line_input_field_rect_size());
  r.setFillColor(sf::Color::Red);
  window.draw(r);
}

void Block::Render() {
  sf::Vector2f pos = position;

  float spacing = 5;

  float padding_left = 5, padding_right = 5;
  float padding_up = 5, padding_down = 5;

  sf::Vector2f block_size{0.0f, 45.0f};
  sf::Vector2f block_position{pos.x - padding_left, pos.y - padding_up};

  // TODO: Update only if dirty.
  // TODO: Cache all.
  // In some function like Recompute Rect() {
  for (auto child : childrens) {
    if (child.type == NODE_TYPE::LABEL) {
      block_size.x += text_rect_size(child.text).x;
      block_size.x += spacing;
    }

    if (child.type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
      block_size.x += line_input_field_rect_size().x;
      block_size.x += spacing;
    }
  }

  // Margins
  // Account for the block position decreased during padding.
  block_size.x += padding_left + padding_right;
  block_size.y += padding_up + padding_down;

  sf::RectangleShape block;
  block.setPosition(block_position);
  block.setSize(block_size);
  block.setFillColor(sf::Color::Green);
  window.draw(block);

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
