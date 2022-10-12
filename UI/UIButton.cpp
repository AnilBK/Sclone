#include "UIButton.hpp"

sf::Vector2f UIButton::getPosition() {
  return text.getRectPosition();
  // return text.getPosition();
}

void UIButton::setPosition(sf::Vector2f pos) { text.setPosition(pos); }

sf::Vector2f UIButton::text_size() { return text.rect_size(); }

sf::Vector2f UIButton::rect_size() {
  sf::Vector2f padding{5.0f, 5.0f};
  return text_size() + (padding * 2.0f);
}

void UIButton::RenderDebug() {
  sf::RectangleShape debug_shape;
  // debug_shape.setPosition(text.getGlobalBounds().left,
  //   text.getGlobalBounds().top);
  debug_shape.setPosition(getPosition());
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
}

void UIButton::Render() {
  // RenderDebug();

  if (!is_flat) {
    sf::RectangleShape r;
    sf::Vector2f padding{5.0f, 5.0f};
    r.setPosition(getPosition() - padding);
    r.setSize(rect_size());
    r.setFillColor(button_fill_color);
    // A shadow when a button is highlighted.
    if (mouse_over) {
      r.setOutlineThickness(2.5);
      r.setOutlineColor(sf::Color(31, 142, 255, 255));
      if (!clicked) {
        r.setFillColor(sf::Color(0, 220, 0));
      }
    }
    // else {
    // r.setOutlineThickness(0.0);
    // }
    window.draw(r);
  }

  text.Render();
}

void UIButton::handle_inputs(sf::Event event) {
  mouse_over = is_mouse_over();

  if (!mouse_over) {
    return;
  }

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    clicked = !clicked;
    if (clicked_callback) {
      clicked_callback();
    }
  }
}
