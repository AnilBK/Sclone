#include "Button.hpp"
#include "../Globals.hpp"
#include "../Utils.hpp"
#include <SFML/Graphics.hpp>

void Button::set_text(std::string &str) { text.setString(str); }

sf::Vector2f Button::getPosition() { return rectangle.getPosition(); }

void Button::setPosition(sf::Vector2f pos) {
  rectangle.setPosition(pos);

  auto rectangle_centre =
      getPosition() + (rectangle.getGlobalBounds().getSize() / 2.f);

  // Set origin of the text to the top left.
  auto text_top_left =
      MATH_UTILITIES::round(text.getLocalBounds().getPosition());
  text.setOrigin(text_top_left);

  // Subtract the text's centre so that, the text's centre lies in rectangle's
  // centre. If we don't subtract text's center then, text's top left would be
  // in the centre of the rectangle.
  auto text_centre = text.getGlobalBounds().getSize() / 2.f;
  text.setPosition(rectangle_centre - text_centre);

  if (text_align == TEXT_ALIGN::LEFT) {
    text.setPosition({getPosition().x + 10.0F, text.getPosition().y});
  }
}

void Button::set_pressed(bool p_pressed) {
  pressed = p_pressed;

  if (pressed) {
    rectangle.setFillColor(pressed_fill_color);
    text.setFillColor(sf::Color(0, 136, 204));
  } else {
    rectangle.setFillColor(default_fill_color);
    text.setFillColor(sf::Color::Black);
  }
}

void Button::set_button_size(sf::Vector2f new_size) {
  rectangle.setSize(new_size);
}

sf::Vector2f Button::rect_size() { return rectangle.getSize(); }

void Button::set_outline_thickness(float p_thickness) {
  rectangle.setOutlineThickness(p_thickness);
}

Button::Button(const std::string &btn_text, sf::Vector2f pos,
               TEXT_ALIGN p_text_align) {
  text.setFont(button_font);
  text.setCharacterSize(DEFAULT_TEXT_FONT_SIZE);
  text.setString(btn_text);
  text.setFillColor(sf::Color(101, 90, 124));

  set_outline_thickness(2.0F);
  rectangle.setOutlineColor(sf::Color(171, 146, 191));
  rectangle.setFillColor(default_fill_color);
  rectangle.setOrigin(sf::Vector2f(0.0F, 0.0F));

  text_align = p_text_align;

  if (text_align == TEXT_ALIGN::EXPAND_BUTTON_TO_TEXT) {
    auto text_size = text.getGlobalBounds().getSize() + sf::Vector2f(10, 10);
    set_button_size(text_size);
  } else {
    set_button_size({130.0F, 30.0F});
  }

  setPosition(pos);
}

void Button::Render() {
  // TODO : Optimize this.
  sf::Color render_color = default_fill_color;

  if (pressed) {
    render_color = pressed_fill_color;
  }

  if (mouse_over) {
    render_color = sf::Color{
        static_cast<sf::Uint8>(render_color.r * 0.9),
        static_cast<sf::Uint8>(render_color.g * 0.9),
        static_cast<sf::Uint8>(render_color.b * 0.9),
    };
  }

  rectangle.setFillColor(render_color);

  window.draw(rectangle);
  window.draw(text);
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
      clicked_callback();
    }
  }
}
