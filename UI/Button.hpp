#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "UIBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

inline sf::Font button_font;

class Button : public UIBaseClass {
private:
  sf::Text text;
  sf::RectangleShape rectangle;

  enum TEXT_ALIGN { CENTER, LEFT, EXPAND_BUTTON_TO_TEXT } text_align;

  bool mouse_over = false;
  bool clicked = false;
  bool pressed = false;

public:
  std::function<void()> clicked_callback;

  void set_pressed(bool p_pressed);

  sf::Vector2f getPosition() override;

  void setPosition(sf::Vector2f) override;

  sf::Vector2f rect_size() override;

  Button(const std::string &btn_text,
         sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

  void Render() override;

  void set_button_size(sf::Vector2f new_size);

  void handle_inputs(sf::Event event) override;

  void set_text(std::string &str);
};

#endif // BUTTON_HPP
