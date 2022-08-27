#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#define ERR_FAIL_COND_CRASH(m_cond, m_fail_message)                            \
  if (!m_cond) {                                                               \
    std::cout << m_fail_message << "\n";                                       \
    exit(1);                                                                   \
  }

enum NODE_TYPE { LABEL, LINE_INPUT_ATTACH_FIELD, BUTTON };

struct NODE {
  std::string text;
  NODE_TYPE type;
};

extern sf::Vector2i mouse_position;

extern sf::Font font;
extern sf::Text draw_text_label;

extern sf::RenderWindow window;

template <class sprite_type> bool isMouseOverSprite(sprite_type sprite) {
  return sprite.getGlobalBounds().contains(mouse_position.x, mouse_position.y);
}

static void init_global_font_and_label() {
  ERR_FAIL_COND_CRASH(font.loadFromFile("alaska.ttf"), "Error Loading Font.");
  draw_text_label.setFont(font);
  draw_text_label.setCharacterSize(32);
  draw_text_label.setPosition(sf::Vector2f(20, 20));
  draw_text_label.setFillColor(sf::Color::Black);
}

static sf::Vector2f text_rect_size(const std::string str) {
  draw_text_label.setString(str);
  return {draw_text_label.getGlobalBounds().width,
          draw_text_label.getGlobalBounds().height};
}

static void draw_text(const std::string &str, const sf::Vector2f p_position) {
  draw_text_label.setString(str);
  draw_text_label.setPosition(p_position);
  window.draw(draw_text_label);
}

#endif
