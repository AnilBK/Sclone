#include "UIButtonLegacy.hpp"

sf::Vector2f UIButtonLegacy::getPosition() {
  return text.get_actual_text_position();
  // return text.getPosition();
}

void UIButtonLegacy::setPosition(sf::Vector2f pos) { text.setPosition(pos); }

sf::Vector2f UIButtonLegacy::text_size() { return text.rect_size(); }

sf::Vector2f UIButtonLegacy::rect_size() {
  return text_size() + (padding * 2.0f);
}

void UIButtonLegacy::fit_to_size(sf::Vector2f new_size) {
  auto diff = new_size - text_size();
  diff *= 0.5f;
  padding = diff;
}

void UIButtonLegacy::RenderDebug() {
  sf::RectangleShape debug_shape;

  // debug_shape.setPosition(text.getGlobalBounds().getPosition());

  debug_shape.setPosition(getPosition());
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
}

void UIButtonLegacy::Render() {
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

void UIButtonLegacy::RenderTo(sf::RenderWindow &target_window) {
  // Same as above, but takes target window as reference.
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
    target_window.draw(r);
  }

  target_window.draw(text.get_label());
  // text.Render();
}

void UIButtonLegacy::handle_inputs(sf::Event event) {
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

void UIButtonLegacy::handle_inputs_to(sf::Event event,
                                      sf::RenderWindow &target_window) {
  mouse_over = is_mouse_over(target_window);

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