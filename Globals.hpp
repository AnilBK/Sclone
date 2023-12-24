#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#ifdef USE_WIN32_FILE_PICKER
#include "Editor/Windows/FilePickerWindowWin32.hpp"
#else
#include "Editor/Windows/FilePickerWindow.hpp"
#endif

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

// Crash if the provided condition is true.
#define ERR_CRASH_IF(m_cond, m_fail_message)                                   \
  if (m_cond) {                                                                \
    std::cout << "\"" << __FILE__ << "\" :: " << __LINE__ << "\t"              \
              << m_fail_message << "\n";                                       \
    exit(1);                                                                   \
  }

#define SHOW_ERROR_ALERT(m_message)                                            \
  std::string err_msg = std::string(__FILE__) +                                \
                        "\" :: " + std::to_string(__LINE__) + "\n" +           \
                        m_message;                                             \
  ALERT(err_msg);

// Break from a function if the string contains a space.
#define RETURN_IF_STRING_HAS_SPACE(m_string, m_message)                        \
  for (const auto &ch : m_string) {                                            \
    if (ch == ' ') {                                                           \
      std::string msg("[Error] \"");                                           \
      msg += std::string(__FILE__) + "\" :: " + std::to_string(__LINE__) +     \
             "\n" + m_message;                                                 \
      ALERT(msg);                                                              \
      return;                                                                  \
    }                                                                          \
  }

enum NODE_TYPE {
  PLACEHOLDER,
  BLOCK_ATTACH_NODE,
  LABEL,
  LINE_INPUT_ATTACH_FIELD,
  PICK_WITH_MOUSE,
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
extern sf::RenderWindow window;

constexpr unsigned int DEFAULT_TEXT_FONT_SIZE = 20;
constexpr bool DEBUG_BLOCK_SPAWN_STATS = false;

inline sf::View *editor_view;

#ifdef USE_WIN32_FILE_PICKER
inline FilePickerWindowWin32 file_picker;
#else
inline FilePickerWindow file_picker;
#endif

[[nodiscard]] static inline sf::Vector2f
get_mouse_position(const sf::RenderWindow &p_window) {
  sf::Vector2i m_pos = sf::Mouse::getPosition(p_window);
  auto mouse_world_pos = p_window.mapPixelToCoords(m_pos);
  return mouse_world_pos;
}

[[nodiscard]] static inline sf::Vector2f get_mouse_position() {
  return get_mouse_position(window);
}

template <class sprite_type> bool isMouseOverSprite(sprite_type sprite) {
  return sprite.getGlobalBounds().contains(get_mouse_position());
}

template <class sprite_type> bool isMouseOverSprite(sprite_type *sprite) {
  // NOTE : nullptr cases are to be checked by the callers.
  return sprite->getGlobalBounds().contains(get_mouse_position());
}

[[nodiscard]] static bool isMouseOverRect(const sf::FloatRect rect) {
  return rect.contains(get_mouse_position());
}

[[nodiscard]] static bool isMouseOverRect(const sf::FloatRect rect,
                                          const sf::RenderWindow &p_window) {
  return rect.contains(get_mouse_position(p_window));
}

[[nodiscard]] static inline sf::Vector2f get_mouse_position_wrt_2d_editor() {
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, *editor_view);
  return worldPos;
}

/// @brief Check if cached mouse position is inside the given rect.
/// @param rect Rect to check.
/// @return True if mouse is over.
[[nodiscard]] static inline bool
isCachedMousePosOverRect(const sf::FloatRect rect) {
  sf::Vector2f position = rect.getPosition();
  sf::Vector2f size = rect.getSize();

  return (mouse_position.x >= position.x &&
          mouse_position.x <= position.x + size.x) &&
         (mouse_position.y >= position.y &&
          mouse_position.y <= position.y + size.y);

  // We could use this VV
  //  return rect.contains(static_cast<sf::Vector2f>(mouse_position));
  // but it has certain checks to handle 'Rectangles with negative dimensions'.
  // We don't need that.
}

static void setCursor(sf::Cursor::Type cursor_type) {
  sf::Cursor cursor;
  if (cursor.loadFromSystem(cursor_type)) {
    window.setMouseCursor(cursor);
  }
}

static inline void draw_line(sf::Vector2f from, sf::Vector2f to,
                             sf::Color color) {
  sf::Vertex vertices[2] = {sf::Vertex(from, color), sf::Vertex(to, color)};
  window.draw(vertices, 2, sf::Lines);
}

#endif
