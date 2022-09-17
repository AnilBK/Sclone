#ifndef DROP_DOWN_HPP
#define DROP_DOWN_HPP

#include "../Globals.hpp"
#include <SFML/Graphics.hpp>

class DropDown {

private:
  int currently_selected_index = 0;
  bool mouse_over = false;
  int mouse_over_index = -1;

public:
  bool dropdown_clicked = false;

  std::vector<std::string> items = {"Item 0", "Item 1", "Item 2"};

  sf::Vector2f position = sf::Vector2f(410, 40);

  bool is_mouse_over();
  std::string get_text() { return items.at(currently_selected_index); }
  std::string get_text(int index) { return items.at(index); }

  sf::Vector2f rect_size() {
    return text_rect_size(get_text()) + sf::Vector2f(0, 10);
  };

  void handle_inputs(sf::Event event);
  void Render();
};

#endif
