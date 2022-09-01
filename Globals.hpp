#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#define ERR_FAIL_COND_CRASH(m_cond, m_fail_message)                            \
  if (!m_cond) {                                                               \
    std::cout << m_fail_message << "\n";                                       \
    exit(1);                                                                   \
  }

enum NODE_TYPE { PLACEHOLDER, LABEL, LINE_INPUT_ATTACH_FIELD, BUTTON };

enum BLOCK_TYPES { CONTROL, INSTRUCTION };

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

static sf::Vector2f text_rect_size(const std::string &str) {
  draw_text_label.setString(str);
  return {draw_text_label.getGlobalBounds().width,
          draw_text_label.getGlobalBounds().height};
}

static void draw_text(const std::string &str, const sf::Vector2f p_position) {
  draw_text_label.setString(str);
  draw_text_label.setPosition(p_position);
  window.draw(draw_text_label);
}

static sf::FloatRect merge_rects(sf::FloatRect rect, sf::FloatRect p_rect) {
  auto rect_pos = sf::Vector2f(rect.left, rect.top);
  auto rect_size = sf::Vector2f(rect.width, rect.height);

  auto p_rect_pos = sf::Vector2f(p_rect.left, p_rect.top);
  auto p_rect_size = sf::Vector2f(p_rect.width, p_rect.height);

  sf::Vector2f merged_rect_position = {std::min(p_rect_pos.x, rect_pos.x),
                                       std::min(p_rect_pos.y, rect_pos.y)};

  sf::Vector2f merged_rect_size = {
      std::max(p_rect_pos.x + p_rect_size.x, rect_pos.x + rect_size.x) -
          merged_rect_position.x,
      std::max(p_rect_pos.y + p_rect_size.y, rect_pos.y + rect_size.y) -
          merged_rect_position.y};

  return sf::FloatRect(merged_rect_position, merged_rect_size);
}

#endif
