#include "GlobalAlertWindow.hpp"

void GlobalAlertWindow::Init() {
  label_font.loadFromFile("alaska.ttf");
  message_label.setFont(label_font);
  message_label.setCharacterSize(30);
  message_label.setPosition(sf::Vector2f(20, 20));
  message_label.setFillColor(sf::Color::Black);
}

GlobalAlertWindow::GlobalAlertWindow() {
  Init();
  message_label.setString("Window Alert Message Here.");
  ok_btn.clicked_callback = [this]() {
    is_active = false;
    alert_window.close();
  };
}

void GlobalAlertWindow::PopupWithMessage(const std::string &message) {
  is_active = true;
  message_label.setString(message);

  // TODO ??? Do this automatically with UI Containers.
  // TODO ??? UI Containers all Render to default window, so change them to be
  // window independent.

  auto message_label_size =
      sf::Vector2f(message_label.getGlobalBounds().width,
                   message_label.getGlobalBounds().height);

  ok_btn.setPosition(sf::Vector2f(20, message_label.getPosition().y +
                                          message_label_size.y + 20));

  float final_h = ok_btn.getPosition().y + ok_btn.rect_size().y + 20;

  alert_window.create(sf::VideoMode(message_label_size.x, final_h),
                      "Important Message!!", sf::Style::Titlebar);

  alert_window.setVisible(true);
  alert_window.requestFocus();
}

void GlobalAlertWindow::MainLoop() {
  if (!is_window_active()) {
    return;
  }

  if (!alert_window.hasFocus()) {
    alert_window.requestFocus();
  }

  sf::Event event;
  while (alert_window.pollEvent(event)) {
    // Handle events for alert window here.
    ok_btn.handle_inputs_to(event, alert_window);
  }

  alert_window.clear(sf::Color::White);
  alert_window.draw(message_label);
  ok_btn.RenderTo(alert_window);
  alert_window.display();
}
