#include "TransformGizmo2D.hpp"
#include "../Nodes/SpriteNode.hpp"
#include <cmath>

bool TransformGizmo2D::is_gizmo_selected() {
  return current_gizmo_state != GIZMO_SELECT_STATE::NONE;
}

void TransformGizmo2D::setTargetEditorSprite(
    EditorSprite *p_target_editor_sprite) {
  target_editor_sprite = p_target_editor_sprite;
}

void TransformGizmo2D::_undrag_gizmos() {
  if (is_gizmo_selected()) {
    current_gizmo_state = GIZMO_SELECT_STATE::NONE;
    setCursor(sf::Cursor::Arrow);
  }
}

void TransformGizmo2D::_update_gizmo() {
  auto target_sprite = target_editor_sprite->get_node();

  switch (current_gizmo_state) {
  case GIZMO_SELECT_STATE::X: {
    const sf::Color x_line_color = sf::Color::Red;

    auto y_pos = target_sprite->getPosition().y;
    float win_width = window.getView().getSize().x;
    auto left_most_x = window.getView().getCenter().x - win_width / 2;

    draw_line({left_most_x, y_pos}, {win_width, y_pos}, x_line_color);

    target_sprite->setPosition(get_mouse_position().x - 100.0f, y_pos);

    if (translation_updated_callbacks) {
      // update translation = true.
      // update scale = false.
      translation_updated_callbacks(true, false);
    }

  } break;

  case GIZMO_SELECT_STATE::Y: {
    const sf::Color y_line_color = sf::Color::Green;

    auto x_pos = target_sprite->getPosition().x;
    float win_height = window.getView().getSize().y;
    auto top_most_y = window.getView().getCenter().y - win_height / 2;

    draw_line({x_pos, top_most_y}, {x_pos, win_height}, y_line_color);

    target_sprite->setPosition(x_pos, get_mouse_position().y + 100.0f);

    if (translation_updated_callbacks) {
      translation_updated_callbacks(true, false);
    }
  } break;

  case GIZMO_SELECT_STATE::CENTER: {
    target_sprite->setPosition(get_mouse_position());

    if (translation_updated_callbacks) {
      translation_updated_callbacks(true, false);
    }
  } break;

  case GIZMO_SELECT_STATE::SCALE: {
    auto sprite = dynamic_cast<SpriteNode *>(target_sprite);
    if (sprite) {
      sf::Vector2f distance = get_mouse_position() - sprite->getPosition();
      auto texture_size = sprite->get_shape().getTexture()->getSize();

      float x_scale_factor =
          fabs(distance.x) / static_cast<float>(texture_size.x);
      float y_scale_factor =
          fabs(distance.y) / static_cast<float>(texture_size.y);

      sprite->get_shape().setScale(x_scale_factor * 2, y_scale_factor * 2);

      if (translation_updated_callbacks) {
        // update translation = false.
        // update scale = true.
        // translation_updated_callbacks(false, true);
      }
    }
  } break;

  case GIZMO_SELECT_STATE::NONE:
    break;
  }
}

void TransformGizmo2D::_draw_gizmo() {
  if (is_gizmo_selected()) {
    return;
  }

  auto target_sprite = target_editor_sprite->get_node();

  auto line_start = target_sprite->getPosition();

  //////////////////////////////////////////////////////////////////////
  //                      The X-Line.                                 //
  //////////////////////////////////////////////////////////////////////
  auto x_line_end = line_start + sf::Vector2f(100.0f, 0.0f);
  const sf::Color x_line_color = sf::Color::Red;
  draw_line(line_start, x_line_end, x_line_color);

  const sf::Vector2f gizmo_pick_size = sf::Vector2f(15.0f, 15.0f);

  sf::RectangleShape x;
  x.setSize(gizmo_pick_size);
  x.setPosition(x_line_end - sf::Vector2f(0.0f, x.getSize().y * 0.5f));
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

  draw_line(line_start, y_line_end, y_line_color);

  sf::RectangleShape y;
  y.setSize(gizmo_pick_size);
  y.setPosition(y_line_end - y.getSize() * 0.5f);
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
  c.setPosition(line_start - gizmo_pick_size * 0.5f);
  c.setFillColor(sf::Color::White);
  if (isMouseOverSprite(c) ||
      current_gizmo_state == GIZMO_SELECT_STATE::CENTER) {
    c.setOutlineThickness(2.0f);
    c.setOutlineColor(sf::Color::Black);
  }
  window.draw(c);

  //////////////////////////////////////////////////////////////////////
  //                      The box at the bottom right.                //
  //////////////////////////////////////////////////////////////////////
  // Dragging it will scale the sprite.
  sf::RectangleShape br;
  br.setSize(gizmo_pick_size);
  br.setPosition(line_start +
                 (target_sprite->getGlobalBounds().getSize() * 0.5F) -
                 (gizmo_pick_size * 0.5f));
  br.setFillColor(sf::Color::Blue);
  if (isMouseOverSprite(br) ||
      current_gizmo_state == GIZMO_SELECT_STATE::SCALE) {
    br.setOutlineThickness(2.0f);
    br.setOutlineColor(sf::Color::Black);
  }
  window.draw(br);

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
  } else if (isMouseOverSprite(br)) {
    current_gizmo_state = GIZMO_SELECT_STATE::SCALE;
  }

  if (is_gizmo_selected()) {
    if (current_gizmo_state == GIZMO_SELECT_STATE::SCALE) {
      setCursor(sf::Cursor::SizeTopLeftBottomRight);
    } else {
      setCursor(sf::Cursor::SizeAll);
    }
  }
}

void TransformGizmo2D::Render() {
  _draw_gizmo();
  _update_gizmo();

  // Reset the gizmo selection.
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
    _undrag_gizmos();
  }
}
