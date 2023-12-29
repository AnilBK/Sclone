#ifndef GLOBAL_ALERT_WINDOW
#define GLOBAL_ALERT_WINDOW

#include "../../UI/Button.hpp"
#include <SFML/Graphics.hpp>
#include <string>

/// @brief This message indicates that new AlertWindow popup should be spawned.
inline bool new_alert_requested = false;

/// @brief The required message for the popup window.
inline std::string alert_message;

/// @brief Seperate window for alerts.
inline sf::RenderWindow alert_window;

inline void ALERT(const std::string &message) {
  new_alert_requested = true;
  alert_message = message;
}

class GlobalAlertWindow {
private:
  bool is_active = false;

  // TODO: Use Label for this.
  sf::Font label_font;

  sf::Text message_label;
  Button ok_btn = Button("Okay");

  void Init();

public:
  bool is_window_active() { return is_active; }

  GlobalAlertWindow();

  void handle_inputs(sf::Event event) {}

  void PopupWithMessage(const std::string &message);

  void MainLoop();
};

#endif
