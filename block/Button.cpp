
#include "Button.hpp"

bool Button::is_mouse_over() {
  sf::Vector2f size = rect_size();
  return isCachedMousePosOverRect(sf::FloatRect(position, size));
}

void Button::handle_inputs(sf::Event event) {
  mouse_over = is_mouse_over();

  if (!mouse_over) {
    return;
  }

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    clicked = !clicked;
    if (clicked_callback) {
      //    std::cout << "Button Pressed. \n";
      clicked_callback();
    }
  }
}

void Button::_draw_button() {
  sf::RectangleShape r;
  r.setPosition(position);
  r.setSize(rect_size());
  r.setFillColor(button_fill_color);
  // A shadow when a button is highlighted.
  if (mouse_over) {
    r.setOutlineThickness(2.5);
    r.setOutlineColor(sf::Color(31, 142, 255, 255));
  } else {
    r.setOutlineThickness(0.0);
  }
  window.draw(r);
}

void Button::_draw_text() { window.draw(text); }

void Button::Render() {
  _draw_button();
  _draw_text();
}

std::string Button::get_text() { return text.getString(); }
