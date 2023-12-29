#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include "UIBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

inline sf::Font button_font;

enum TEXT_ALIGN { CENTER, LEFT, EXPAND_BUTTON_TO_TEXT };

class UIButton : public UIBaseClass {
private:
  sf::Text text;
  sf::RectangleShape rectangle;

  bool mouse_over = false;
  bool pressed = false;

  void reposition();

public:
  bool clicked = false;
  TEXT_ALIGN text_align;

  sf::Color default_fill_color = sf::Color(150, 144, 80),
            pressed_fill_color = sf::Color(206, 207, 131);

  std::function<void()> clicked_callback;

  void deselect() { clicked = false; }

  bool is_clicked() { return clicked; }

  void set_button_size(sf::Vector2f new_size);

  void set_pressed(bool p_pressed);

  void set_outline_thickness(float p_thickness);

  void set_text(const std::string &str);

  void set_text_align(TEXT_ALIGN p_text_align);

  sf::Vector2f getPosition() override;

  void setPosition(sf::Vector2f) override;

  sf::Vector2f rect_size() override;

  UIButton(const std::string &btn_text);

  void RenderDebug() override;

  void Render() override;

  void RenderTo(sf::RenderWindow &p_target_window);

  void handle_inputs(sf::Event event) override;

  void handle_inputs_to(sf::Event event, sf::RenderWindow &target_window);
};

#endif // BUTTON_HPP
