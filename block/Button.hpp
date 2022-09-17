#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../Globals.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class Button {

private:
  sf::Text text;
  sf::Vector2f position = {0.0f, 0.0f};
  float padding_h = 15, padding_v = 15;

public:
  bool mouse_over = true;
  std::function<void()> clicked_callback;

  sf::Color button_fill_color = sf::Color::Green;

  Button(std::string btn_text, sf::Font &font) {
    text.setString(btn_text);
    setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(26);
  }

  std::string get_text();

  sf::Vector2f get_text_size() {
    auto bounds = text.getGlobalBounds();
    return {bounds.width, bounds.height + 10};
    // BUG:This isn't returning correct height. vvv
    return {bounds.width, bounds.height};
  }

  void setFont(sf::Font &font) { text.setFont(font); }

  void setPosition(sf::Vector2f pos) {
    position = pos;
    text.setPosition(pos + sf::Vector2f{padding_h, padding_v});
  }

  sf::Vector2f rect_size() {
    sf::Vector2f text_box_size =
        get_text_size() + sf::Vector2f{padding_h * 2, padding_v * 2};
    return text_box_size;
    // Like in HTML, the sizes of the button can be increased with padding.
  }

  void _draw_button();
  void _draw_text();
  bool is_mouse_over();
  void handle_inputs(sf::Event event);
  void Render();
};

#endif // BUTTON_HPP
