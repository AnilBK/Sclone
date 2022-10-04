#ifndef UI_LINE_INPUT_HPP
#define UI_LINE_INPUT_HPP

#include "Label.hpp"
#include "UIBaseClass.hpp"
#include <functional>

class UILineInput : public UIBaseClass {
private:
  bool clicked = false;
  bool mouse_over = false;

  void _draw_button(const sf::Vector2f p_position, const sf::Vector2f p_size,
                    sf::Color color = sf::Color::White);

public:
  sf::Color button_fill_color = sf::Color::Green;
  std::function<void()> clicked_callback;
  // Things to do when enter is pressed on the Keyboard.
  std::function<void()> enter_pressed_callback;

  Label text;

  std::string input_text;
  std::string prefix = "";
  sf::Clock clock;

  bool show_cursor = true;

  sf::Time text_effect_time;

  bool allows_multi_line = false;
  bool line_input_active = false;

  sf::Vector2f min_size = {75, 40};

  UILineInput(const std::string &btn_text) {
    text.setText(btn_text);
    input_text = text.getText();
  }

  std::string get_text();

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void RenderDebug() override;
  void Render() override;

  void handle_inputs(sf::Event event) override;
};
#endif // UI_BUTTON
