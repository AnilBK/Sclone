#include "UIButton.hpp"
#include "../Globals.hpp"
#include "../Utils.hpp"
#include "UILabel.hpp"
#include <SFML/Graphics.hpp>

void UIButton::set_button_size(sf::Vector2f new_size) {
  rectangle.setSize(new_size);
  reposition();
}

void UIButton::set_button_width(float p_width) {
  sf::Vector2f current_size = rect_size();
  set_button_size(sf::Vector2f(p_width, current_size.y));
}

void UIButton::set_border_radius(float radius) {
  rectangle.setCornersRadius(radius);
  reposition();
}

void UIButton::set_pressed(bool p_pressed) {
  pressed = p_pressed;
  rectangle.setFillColor(pressed ? pressed_fill_color : default_fill_color);
  text.setFillColor(sf::Color::Black);
}

void UIButton::set_outline_thickness(float p_thickness) {
  rectangle.setOutlineThickness(p_thickness);
}

void UIButton::set_text(const std::string &str) {
  text.setString(str);
  reposition();
}

void UIButton::set_text_align(TEXT_ALIGN p_text_align) {
  text_align = p_text_align;
  if (text_align == TEXT_ALIGN::EXPAND_BUTTON_TO_TEXT) {
    auto text_size = UILabel::get_consistent_local_bounds(text).getSize() +
                     sf::Vector2f(10, 10);
    set_button_size(text_size);
  } else {
    reposition();
  }
}

void UIButton::reposition() {
  auto rectangle_centre = getPosition() + (rect_size() / 2.f);
  auto consistent_bounds = UILabel::get_consistent_local_bounds(text);

  // Set origin of the text to the top left
  sf::Vector2f text_top_left(consistent_bounds.left, consistent_bounds.top);
  text.setOrigin(MATH_UTILITIES::round(text_top_left));

  // Center the text inside the button container
  auto text_centre = consistent_bounds.getSize() / 2.f;
  text.setPosition(MATH_UTILITIES::round(rectangle_centre - text_centre));

  // If left aligned, just snap the X position to the left padding.
  // Because origin X is at the left, this safely aligns the left edge without
  // shifting.
  if (text_align == TEXT_ALIGN::LEFT) {
    text.setPosition({getPosition().x + left_padding, text.getPosition().y});
  }
}

sf::Vector2f UIButton::getPosition() { return rectangle.getPosition(); }

void UIButton::setPosition(sf::Vector2f pos) {
  auto diff = getPosition() - pos;
  if (MATH_UTILITIES::is_approx_zero(diff)) {
    return;
  }
  rectangle.setPosition(pos);
  reposition();
}

sf::Vector2f UIButton::rect_size() { return rectangle.getSize(); }

UIButton::UIButton(const std::string &btn_text) {
  text.setFont(button_font);
  text.setCharacterSize(DEFAULT_TEXT_FONT_SIZE);
  text.setString(btn_text);
  text.setFillColor(sf::Color::Black);

  rectangle.setCornerPointCount(15);
  rectangle.setCornersRadius(0.0f);

  set_outline_thickness(0.0F);
  rectangle.setOutlineColor(sf::Color(171, 146, 191));
  rectangle.setFillColor(default_fill_color);
  rectangle.setOrigin(sf::Vector2f(0.0F, 0.0F));

  set_text_align(TEXT_ALIGN::EXPAND_BUTTON_TO_TEXT);
  setPosition(sf::Vector2f(0.0F, 0.0F));
}

void UIButton::RenderDebug() {
  sf::RectangleShape debug_shape;
  debug_shape.setPosition(getPosition());
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
}

void UIButton::Render() { RenderTo(window); }

void UIButton::handle_inputs(sf::Event event) {
  handle_inputs_to(event, window);
}

void UIButton::RenderTo(sf::RenderWindow &p_target_window) {
  sf::Color render_color = pressed ? pressed_fill_color : default_fill_color;
  if (mouse_over) {
    render_color = sf::Color{
        static_cast<sf::Uint8>(render_color.r * 0.9),
        static_cast<sf::Uint8>(render_color.g * 0.9),
        static_cast<sf::Uint8>(render_color.b * 0.9),
    };
  }
  rectangle.setFillColor(render_color);
  p_target_window.draw(rectangle);
  p_target_window.draw(text);
}

void UIButton::handle_inputs_to(sf::Event event,
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