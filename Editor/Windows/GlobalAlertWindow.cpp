#include "GlobalAlertWindow.hpp"

void GlobalAlertWindow::Init() {
  label_font.loadFromFile("OpenSans-Regular.ttf");
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
  ok_btn.default_fill_color = sf::Color::Green;
  ok_btn.set_text_align(TEXT_ALIGN::EXPAND_BUTTON_TO_TEXT);
  ok_btn.set_outline_thickness(0.0F);
}

void GlobalAlertWindow::PopupWithMessage(const std::string &message) {
  is_active = true;
  message_label.setString(message);

  // TODO ??? Do this automatically with UI Containers.
  // TODO ??? UI Containers all Render to default window, so change them to be
  // window independent.

  auto message_label_size = message_label.getGlobalBounds().getSize();

  ok_btn.setPosition(sf::Vector2f(20, message_label.getPosition().y +
                                          message_label_size.y + 20));

  // Window width should contain the message label, which is the widest UIItem
  // in this window, with 20 right padding.
  float final_w = message_label.getPosition().x + message_label_size.x + 20;

  // Window height should contain the ok_btn, as the ok_btn is the last UIItem
  // in the window, with 20 pixels remaining below as padding.
  float final_h = ok_btn.getPosition().y + ok_btn.rect_size().y + 20;

  alert_window.create(sf::VideoMode(final_w, final_h), "Important Message!!",
                      sf::Style::Titlebar);

  alert_window.setVisible(true);
  alert_window.requestFocus();
}

void GlobalAlertWindow::MainLoop() {
  if (new_alert_requested) {
    PopupWithMessage(alert_message);
    new_alert_requested = false;
  }

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
