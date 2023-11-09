#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "UIBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

inline sf::Font button_font;

enum TEXT_ALIGN { CENTER, LEFT, EXPAND_BUTTON_TO_TEXT };

class Button : public UIBaseClass {
private:
  sf::Text text;
  sf::RectangleShape rectangle;

  bool mouse_over = false;
  bool pressed = false;

public:
  bool clicked = false;
  TEXT_ALIGN text_align;

  sf::Color default_fill_color = sf::Color(150, 144, 80),
            pressed_fill_color = sf::Color(206, 207, 131);

  std::function<void()> clicked_callback;

  void set_pressed(bool p_pressed);

  sf::Vector2f getPosition() override;

  void setPosition(sf::Vector2f) override;

  sf::Vector2f rect_size() override;

  void set_outline_thickness(float p_thickness);

  Button(const std::string &btn_text,
         sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f),
         TEXT_ALIGN p_text_align = TEXT_ALIGN::LEFT);

  void Render() override;

  void set_button_size(sf::Vector2f new_size);

  void handle_inputs(sf::Event event) override;

  void set_text(std::string &str);
};

#endif // BUTTON_HPP
