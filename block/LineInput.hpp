#ifndef LINE_INPUT_HPP
#define LINE_INPUT_HPP

// https://en.sfml-dev.org/forums/index.php?topic=26927.0

#include <SFML/Graphics.hpp>
#include <locale>

#include "../Globals.hpp"

// GUI Class to get a input field.
class LineInput {

public:
  std::string input_text;
  std::string prefix = "";
  sf::Clock clock;

  bool show_cursor = true;

  sf::Time text_effect_time;

  bool allows_multi_line = false;
  bool line_input_active = true;

  sf::Vector2f min_size = {217, 50};
  sf::Vector2f position = {0.0f, 0.0f};

  LineInput() {}

  std::string get_text();

  sf::RectangleShape _get_rect();

  bool is_mouse_over();

  sf::Vector2f rect_size() {
    sf::Vector2f size = {217, 50};
    sf::Vector2f text_box_size{text_rect_size(get_text())};
    sf::Vector2f btn_size{std::max(text_box_size.x, size.x) + 5,
                          std::max(text_box_size.y, size.y)};
    return btn_size;
  }

  void _draw_button(const sf::Vector2f p_position, const sf::Vector2f p_size,
                    sf::Color color = sf::Color(192, 195, 198, 255));

  void handle_inputs(sf::Event event);

  void Render();
};

#endif // LINE_INPUT_HPP
