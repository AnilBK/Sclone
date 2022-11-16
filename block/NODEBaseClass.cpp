#include "NODEBaseClass.hpp"
#include "Block.hpp"

sf::Vector2f NODEBaseClass::min_size() { return {35.0f, 45.0f}; }

bool NODEBaseClass::is_mouse_over(sf::Vector2f this_nodes_pos) {
  sf::Vector2f size = rect_size();
  return (mouse_position.x >= this_nodes_pos.x &&
          mouse_position.x <= this_nodes_pos.x + size.x) &&
         (mouse_position.y >= this_nodes_pos.y &&
          mouse_position.y <= this_nodes_pos.y + size.y);
}

std::string NODEBaseClass::get_text() { return text; }

void NODEBaseClass::set_text(const std::string &str) { text = str; }

bool NODEBaseClass::left_click_action() { return false; }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f LabelNode::rect_size() {
  return {std::max(text_rect_size(text).x, min_size().x) + 15, min_size().y};
}

void LabelNode::Render(sf::Vector2f pos) { draw_text(text, pos); }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f ButtonNode::rect_size() { return btn.rect_size(); }

bool ButtonNode::left_click_action() {
  pressed = true;
  return true;
}

void ButtonNode::Render(sf::Vector2f pos) {
  btn.setPosition(pos);
  btn.Render();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f LineInputAttachFieldNode::rect_size() {
  return text_area.rect_size();
}

std::string LineInputAttachFieldNode::get_text() {
  return text_area.input_text;
}

void LineInputAttachFieldNode::set_text(const std::string &str) {
  text_area.input_text = str;
}

bool LineInputAttachFieldNode::left_click_action() {
  pressed = true;
  text_area.line_input_active = true;
  return true;
}

void LineInputAttachFieldNode::handle_inputs(sf::Event event) {
  text_area.handle_inputs(event);
}

void LineInputAttachFieldNode::Render(sf::Vector2f pos) {
  text_area.position = pos;
  text_area.Render();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f DropDownNode::rect_size() { return dropdown.rect_size(); }

std::string DropDownNode::get_text() { return dropdown.get_text(); }

bool DropDownNode::left_click_action() {
  // dropdown.dropdown_clicked = true;
  return true;
}

void DropDownNode::handle_inputs(sf::Event event) {
  dropdown.handle_inputs(event);
}

void DropDownNode::Render(sf::Vector2f pos) {
  dropdown.position = pos;
  dropdown.Render();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void BlockAttachNode::_update_internal_sizes() {
  // nullptr validity is already checked.
  // the internal sizes are updated by Block class of those blocks, which aren't
  // nullptrs.
  attached_block->update_children_sizes();

  auto attached_rect = attached_block->full_rect();
  sf::Vector2f rec_size{attached_rect.width, attached_rect.height};

  set_enclosed_rect_size(rec_size);
}

void BlockAttachNode::set_enclosed_rect_size(sf::Vector2f p_size) {
  enclosed_rect_size = p_size;
}

sf::Vector2f BlockAttachNode::rect_size() { return enclosed_rect_size; }

sf::FloatRect BlockAttachNode::_attachable_block_snap_hint_rect() {
  // The attachable block starts from the top left of the vertical 'L' shaped
  // line.
  // What is supposed to be snap highlight.
  auto snap_rect_size = sf::Vector2f(250.0f, 10.0f);
  auto snap_rect_position = _pos + sf::Vector2f{15.0f, 0.0f};
  return sf::FloatRect{snap_rect_position, snap_rect_size};
}

void BlockAttachNode::_show_snap_for_attachable_block() {
  const auto r = _attachable_block_snap_hint_rect();
  // r is _attachable_block_snap_hint_rect.
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width + 45, r.height);

  sf::RectangleShape block_snap_hint;
  block_snap_hint.setPosition(r_pos);
  block_snap_hint.setSize(r_size);

  block_snap_hint.setFillColor(sf::Color::White);
  window.draw(block_snap_hint);
}

bool BlockAttachNode::can_snap_block_inside() {
  return isMouseOverRect(_attachable_block_snap_hint_rect());
}

// The rect size with the L-shaped outlines.
sf::FloatRect BlockAttachNode::rect_size_with_outlines() {
  auto l_shape_pos = _pos;
  sf::Vector2f l_shape_size{15, rect_size().y};
  l_shape_size += sf::Vector2f(0.0f, 45.0f); // Just an extra space.

  sf::FloatRect l_shape_rect(l_shape_pos, l_shape_size);

  if (draw_bottom_part) {
    auto l_shape_bottom_pos = _pos + sf::Vector2f(0.0f, l_shape_size.y);
    auto l_shape_bottom_size = sf::Vector2f(300, 45.0f);

    sf::FloatRect l_shape_bottom_rect(l_shape_bottom_pos, l_shape_bottom_size);

    l_shape_rect = merge_rects(l_shape_rect, l_shape_bottom_rect);
  }

  return l_shape_rect;
}

void BlockAttachNode::Render(sf::Vector2f pos) {
  if (attached_block != nullptr) {
    attached_block->set_position(_pos + sf::Vector2f(15.0f, 0.0f));
  }

  // Draw the long vertical line on the left.
  sf::RectangleShape r;
  r.setPosition(pos);
  r.setSize({15, enclosed_rect_size.y}); //{15, 2 * 45.0f};
  r.setFillColor(sf::Color::Yellow);
  window.draw(r);

  if (!draw_bottom_part) {
    return;
  }

  // The horizontal block on the bottom.
  // Which marks the end of the block.
  sf::RectangleShape r2;
  r2.setPosition(pos + sf::Vector2f(0.0f, r.getSize().y));
  // A block is 45 units.
  r2.setSize({300, 45.0f});
  r2.setFillColor(sf::Color::Yellow);
  window.draw(r2);
}
