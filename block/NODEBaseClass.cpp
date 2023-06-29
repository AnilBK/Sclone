#include "NODEBaseClass.hpp"
#include "../Utils.hpp"
#include "Block.hpp"

sf::Vector2f NODEBaseClass::min_size() { return {35.0f, 45.0f}; }

void NODEBaseClass::RenderDebug() {
  sf::RectangleShape r;
  r.setPosition(_pos);
  r.setSize(rect_size());
  r.setFillColor(sf::Color(0, 255, 0, 200));
  window.draw(r);
}

bool NODEBaseClass::is_mouse_over() {
  sf::Vector2f position = _pos;
  sf::Vector2f size = rect_size();

  return isCachedMousePosOverRect(sf::FloatRect(position, size));
}

void NODEBaseClass::deselect_node() { pressed = false; }

std::string NODEBaseClass::get_text() { return text; }

void NODEBaseClass::set_text(const std::string &str) { text = str; }

bool NODEBaseClass::left_click_action() { return false; }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f LabelNode::rect_size() { return label.get_actual_text_size(); }

void LabelNode::set_position(sf::Vector2f pos) {
  NODEBaseClass::set_position(pos);
  label.setPosition(pos);
}

void LabelNode::Render() { label.Render(); }

void LabelNode::RenderDebug() {
  sf::RectangleShape r;
  r.setPosition(label.get_actual_text_position());
  r.setSize(rect_size());
  r.setFillColor(sf::Color(255, 0, 0, 200));
  window.draw(r);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f ButtonNode::rect_size() { return btn.rect_size(); }

bool ButtonNode::left_click_action() {
  pressed = true;
  return true;
}

void ButtonNode::set_position(sf::Vector2f pos) {
  NODEBaseClass::set_position(pos);
  btn.setPosition(pos);
}

void ButtonNode::handle_inputs(sf::Event event) { btn.handle_inputs(event); }

void ButtonNode::Render() { btn.Render(); }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PickWithMouseNode::deselect_node() {
  pressed = false;
  btn.deselect();
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f LineInputNode::rect_size() { return text_area.rect_size(); }

std::string LineInputNode::get_text() { return text_area.get_text_no_prefix(); }

void LineInputNode::set_text(const std::string &str) {
  text_area.set_text(str);
}

bool LineInputNode::left_click_action() {
  pressed = true;
  text_area.line_input_active = true;
  return true;
}

void LineInputNode::set_position(sf::Vector2f pos) {
  NODEBaseClass::set_position(pos);
  text_area.setPosition(pos);
}

void LineInputNode::handle_inputs(sf::Event event) {
  text_area.handle_inputs(event);
}

void LineInputNode::Render() { text_area.Render(); }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Vector2f DropDownNode::rect_size() { return dropdown.rect_size(); }

std::string DropDownNode::get_text() { return dropdown.get_text(); }

bool DropDownNode::left_click_action() {
  // dropdown.dropdown_clicked = true;
  return true;
}

void DropDownNode::set_position(sf::Vector2f pos) {
  NODEBaseClass::set_position(pos);
  dropdown.setPosition(pos);
}

void DropDownNode::handle_inputs(sf::Event event) {
  dropdown.handle_inputs(event);
}

void DropDownNode::Render() { dropdown.Render(); }

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void BlockAttachNode::_update_internal_sizes() {
  // nullptr validity is already checked.
  // the internal sizes are updated by Block class of those blocks, which aren't
  // nullptrs.
  attached_block->update_children_sizes();

  auto attached_rect = attached_block->full_rect();
  sf::Vector2f rec_size = attached_rect.getSize();

  // Add additional 45 units of space inside the l-shape, so we see some space
  // there.
  // It is to hint other blocks that they can be snapped to it.
  rec_size.y += 45.0f;

  vertical_line.setSize({15, rec_size.y});
  update_size();
}

void BlockAttachNode::update_size() {
  // TODO ?? This can be optimized with some kinds of 'dirty' flag ??

  const auto bottom_line = horizontal_line.getGlobalBounds();

  sf::FloatRect shape_rect = vertical_line.getGlobalBounds();
  shape_rect.width = bottom_line.width;

  // actual_full_rect = l_shape_rect; <- This gives the size of the rect without
  // the attached blocks.

  if (draw_bottom_part) {
    shape_rect.height += bottom_line.height;
  }

  if (attached_block != nullptr) {
    shape_rect =
        MATH_UTILITIES::merge_rects(shape_rect, attached_block->full_rect());
  }

  actual_full_rect = shape_rect;
}

sf::Vector2f BlockAttachNode::rect_size() { return actual_full_rect.getSize(); }

sf::Vector2f BlockAttachNode::_get_attach_block_position() {
  return _pos + sf::Vector2f{15.0f, 0.0f};
}

sf::FloatRect BlockAttachNode::_attachable_block_snap_hint_rect() {
  // The attachable block starts from the top left of the vertical 'L' shaped
  // line.
  // What is supposed to be snap highlight.
  auto snap_rect_size = sf::Vector2f(250.0f, 10.0f);
  auto snap_rect_position = _get_attach_block_position();
  return sf::FloatRect{snap_rect_position, snap_rect_size};
}

void BlockAttachNode::_show_snap_for_attachable_block() {
  const auto r = _attachable_block_snap_hint_rect();
  // r is _attachable_block_snap_hint_rect.
  auto r_pos = r.getPosition();
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

void BlockAttachNode::set_position(sf::Vector2f pos) {
  NODEBaseClass::set_position(pos);
  if (attached_block != nullptr) {
    attached_block->set_position(_get_attach_block_position());
  }

  vertical_line.setPosition(pos);
  horizontal_line.setPosition(pos +
                              sf::Vector2f(0.0f, vertical_line.getSize().y));

  update_size();
}

sf::FloatRect BlockAttachNode::full_rect() { return actual_full_rect; }

void BlockAttachNode::RenderDebug() {
  auto r = full_rect();
  sf::RectangleShape rect;
  rect.setPosition(r.getPosition());
  rect.setSize(r.getSize());
  rect.setFillColor(sf::Color(255, 0, 0, 200));
  window.draw(rect);
}

void BlockAttachNode::Render() {
  window.draw(vertical_line);

  if (draw_bottom_part) {
    window.draw(horizontal_line);
  }
}
