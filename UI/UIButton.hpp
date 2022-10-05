#ifndef UI_BUTTON
#define UI_BUTTON

#include "Label.hpp"
#include "UIBaseClass.hpp"
#include <functional>

class UIButton : public UIBaseClass {
private:
  bool clicked = false;
  bool mouse_over = false;

public:
  sf::Color button_fill_color = sf::Color::Green;
  bool is_flat = false;
  Label text;
  std::function<void()> clicked_callback;

  UIButton(const std::string &btn_text,
           sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f)) {
    text.setText(btn_text);
  }

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void RenderDebug() override;
  void Render() override;

  void handle_inputs(sf::Event event) override;
};
#endif // UI_BUTTON
