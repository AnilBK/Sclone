#include "TransformGizmo2D.hpp"

bool TransformGizmo2D::_is_gizmo_selected() {
  return current_gizmo_state != GIZMO_SELECT_STATE::NONE;
}

void TransformGizmo2D::setTargetSprite(sf::Sprite *target_spr) {
  target_sprite = target_spr;
}

void TransformGizmo2D::_undrag_gizmos() {
  if (_is_gizmo_selected()) {
    current_gizmo_state = GIZMO_SELECT_STATE::NONE;
    setCursor(sf::Cursor::Arrow);
  }
}

void TransformGizmo2D::_draw_gizmo_axes() {
  switch (current_gizmo_state) {
  case GIZMO_SELECT_STATE::X: {
    const sf::Color x_line_color = sf::Color::Red;

    auto y_pos = target_sprite->getPosition().y;
    float width = window.getSize().x;

    sf::Vertex x_vertices[2] = {sf::Vertex({0, y_pos}, x_line_color),
                                sf::Vertex({width, y_pos}, x_line_color)};
    window.draw(x_vertices, 2, sf::Lines);

    target_sprite->setPosition(mouse_position.x - 100.0f, y_pos);
  } break;

  case GIZMO_SELECT_STATE::Y: {
    const sf::Color y_line_color = sf::Color::Green;

    auto x_pos = target_sprite->getPosition().x;
    float height = window.getSize().y;

    sf::Vertex y_vertices[2] = {sf::Vertex({x_pos, 0}, y_line_color),
                                sf::Vertex({x_pos, height}, y_line_color)};
    window.draw(y_vertices, 2, sf::Lines);

    target_sprite->setPosition(x_pos, mouse_position.y + 100.0f);
  } break;

  case GIZMO_SELECT_STATE::CENTER:
    target_sprite->setPosition((sf::Vector2f)mouse_position);
    break;

  case GIZMO_SELECT_STATE::NONE:
    break;
  }
}

void TransformGizmo2D::_draw_gizmo() {
  if (_is_gizmo_selected()) {
    return;
  }

  auto line_start = target_sprite->getPosition();

  //////////////////////////////////////////////////////////////////////
  //                      The X-Line.                                 //
  //////////////////////////////////////////////////////////////////////
  auto x_line_end = line_start + sf::Vector2f(100.0f, 0.0f);
  const sf::Color x_line_color = sf::Color::Red;
  sf::Vertex x_vertices[2] = {sf::Vertex(line_start, x_line_color),
                              sf::Vertex(x_line_end, x_line_color)};
  window.draw(x_vertices, 2, sf::Lines);

  const sf::Vector2f gizmo_pick_size = sf::Vector2f(15.0f, 15.0f);

  sf::RectangleShape x;
  x.setSize(gizmo_pick_size);
  x.setPosition(x_vertices[1].position -
                sf::Vector2f(0.0f, x.getSize().y * 0.5f));
  x.setFillColor(x_line_color);
  if (isMouseOverSprite(x) || current_gizmo_state == GIZMO_SELECT_STATE::X) {
    x.setOutlineThickness(2.0f);
    x.setOutlineColor(sf::Color::Black);
  }
  window.draw(x);

  //////////////////////////////////////////////////////////////////////
  //                      The Y-Line.                                 //
  //////////////////////////////////////////////////////////////////////
  auto y_line_end = line_start - sf::Vector2f(0.0f, 100.0f);
  const sf::Color y_line_color = sf::Color::Green;
  sf::Vertex y_vertices[2] = {sf::Vertex(line_start, y_line_color),
                              sf::Vertex(y_line_end, y_line_color)};
  window.draw(y_vertices, 2, sf::Lines);

  sf::RectangleShape y;
  y.setSize(gizmo_pick_size);
  y.setPosition(y_vertices[1].position - y.getSize() * 0.5f);
  y.setFillColor(y_line_color);
  if (isMouseOverSprite(y) || current_gizmo_state == GIZMO_SELECT_STATE::Y) {
    y.setOutlineThickness(2.0f);
    y.setOutlineColor(sf::Color::Black);
  }
  window.draw(y);

  //////////////////////////////////////////////////////////////////////
  //                      The box at the center.                      //
  //////////////////////////////////////////////////////////////////////
  // Dragging it will drag the sprite to mouse position.
  sf::RectangleShape c;
  c.setSize(gizmo_pick_size);
  c.setPosition(y_vertices[0].position - gizmo_pick_size * 0.5f);
  c.setFillColor(sf::Color::White);
  if (isMouseOverSprite(c) ||
      current_gizmo_state == GIZMO_SELECT_STATE::CENTER) {
    c.setOutlineThickness(2.0f);
    c.setOutlineColor(sf::Color::Black);
  }
  window.draw(c);

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  bool select_gizmo_requested = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (!select_gizmo_requested) {
    return;
  }

  if (isMouseOverSprite(x)) {
    current_gizmo_state = GIZMO_SELECT_STATE::X;
  } else if (isMouseOverSprite(y)) {
    current_gizmo_state = GIZMO_SELECT_STATE::Y;
  } else if (isMouseOverSprite(c)) {
    current_gizmo_state = GIZMO_SELECT_STATE::CENTER;
  }

  if (_is_gizmo_selected()) {
    setCursor(sf::Cursor::SizeAll);
  }
}

void TransformGizmo2D::Render() {
  _draw_gizmo();
  _draw_gizmo_axes();

  // Reset the gizmo selection.
  if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
    _undrag_gizmos();
  }
}
