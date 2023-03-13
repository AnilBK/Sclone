#ifndef GLOBAL_ALERT_WINDOW
#define GLOBAL_ALERT_WINDOW

#include "../../UI/UIButton.hpp"
#include <SFML/Graphics.hpp>

class GlobalAlertWindow {
private:
  bool is_active = false;

  // TODO: Use Label for this.
  sf::Font label_font;

  sf::Text message_label;
  UIButton ok_btn = UIButton("Okay");

  void Init();

public:
  bool is_window_active() { return is_active; }

  GlobalAlertWindow();

  void handle_inputs(sf::Event event) {}

  void PopupWithMessage(const std::string &message);

  void MainLoop();
};

#endif
