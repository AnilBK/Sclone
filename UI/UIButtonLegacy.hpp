#ifndef UI_BUTTON_LEGACY
#define UI_BUTTON_LEGACY

#include "UIBaseClass.hpp"
#include "UILabel.hpp"
#include <functional>

class UIButtonLegacy : public UIBaseClass {
private:
  bool mouse_over = false;
  sf::Vector2f padding{5.0f, 5.0f};

public:
  // VVV Public because TabBar class requires it.
  // TODO: Refactor that class so it doesn't require it to be public.
  bool clicked = false;

  sf::Color button_fill_color = sf::Color::Green;
  bool is_flat = false;
  UILabel text;
  std::function<void()> clicked_callback;

  UIButtonLegacy(const std::string &btn_text,
                 sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f)) {
    text.set_text(btn_text);
  }

  void deselect() { clicked = false; }

  bool is_clicked() { return clicked; }

  sf::Vector2f text_size();
  void fit_to_size(sf::Vector2f new_size);

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void RenderDebug() override;
  void Render() override;
  void RenderTo(sf::RenderWindow &target_window);

  void handle_inputs(sf::Event event) override;
  void handle_inputs_to(sf::Event event, sf::RenderWindow &target_window);
};
#endif // UI_BUTTON_LEGACY
