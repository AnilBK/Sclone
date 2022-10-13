#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

// Crash if the provided condition is true.
#define ERR_FAIL_COND_CRASH(m_cond, m_fail_message)                            \
  if (m_cond) {                                                                \
    std::cout << "\"" << __FILE__ << "\" :: " << __LINE__ << "\t"              \
              << m_fail_message << "\n";                                       \
    exit(1);                                                                   \
  }

// Break from a function if the string contains a space.
#define RETURN_IF_STRING_HAS_SPACE(m_string, m_message)                        \
  for (const auto &ch : m_string) {                                            \
    if (ch == ' ') {                                                           \
      std::cout << "[Error] \"" << __FILE__ << "\" :: " << __LINE__ << " "     \
                << m_message << "\n";                                          \
      return;                                                                  \
    }                                                                          \
  }

enum NODE_TYPE {
  PLACEHOLDER,
  BLOCK_ATTACH_NODE,
  LABEL,
  LINE_INPUT_ATTACH_FIELD,
  BUTTON,
  DROP_DOWN
};

enum BLOCK_TYPES { CONTROL, INSTRUCTION };

// Tab where a block belongs to in the editor.
// This should be according to the order the tabs are pushed in the tab bar.
enum BLOCKS_TAB_NAME {
  TAB_CONTROL = 0,
  TAB_DRAW_PRIMITIVES = 1,
  TAB_MOTION = 2,
  TAB_VARIABLES = 3
};

extern sf::Vector2i mouse_position;

extern sf::Font font;
extern sf::Text draw_text_label;

extern sf::RenderWindow window;

template <class sprite_type> bool isMouseOverSprite(sprite_type sprite) {
  // return sprite.getGlobalBounds().contains(mouse_position.x,
  // mouse_position.y);
  sf::Vector2i m_pos = sf::Mouse::getPosition(window);
  return sprite.getGlobalBounds().contains(m_pos.x, m_pos.y);
}

static void init_global_font_and_label() {
  ERR_FAIL_COND_CRASH(!font.loadFromFile("alaska.ttf"), "Error Loading Font.");
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

static void setCursor(sf::Cursor::Type cursor_type) {
  sf::Cursor cursor;
  if (cursor.loadFromSystem(cursor_type)) {
    window.setMouseCursor(cursor);
  }
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

// https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
static void replaceAll(std::string &s, const std::string &search,
                       const std::string &replace) {
  for (size_t pos = 0;; pos += replace.length()) {
    // Locate the substring to replace
    pos = s.find(search, pos);
    if (pos == std::string::npos)
      break;
    // Replace by erasing and inserting
    s.erase(pos, search.length());
    s.insert(pos, replace);
  }
}

static void remove_first_occurence(std::string &s, char ch) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == ch) {
      s.erase(s.begin() + i);
      break;
    }
  }
}

static void remove_last_occurence(std::string &s, char ch) {
  for (int i = s.length() - 1; i >= 0; i--) {
    if (s[i] == ch) {
      s.erase(s.begin() + i);
      break;
    }
  }
}

#endif
