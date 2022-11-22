#ifndef DROP_DOWN_HPP
#define DROP_DOWN_HPP

#include "../Globals.hpp"
#include <SFML/Graphics.hpp>
#include <initializer_list>

class DropDown {

private:
  int currently_selected_index = 0;
  bool mouse_over = false;
  int mouse_over_index = -1;

  /// @brief The base DropDown button when the DropDown isn't expanded.
  void _draw_base_button();

  /// @brief Draws all the items in the list that the user can pick.
  void _draw_whole_list();

  // @brief The rect size required for the largest item in the list.
  sf::Vector2f largest_item_rect_size;
  void _compute_largest_item_rect_size();

  std::vector<std::string> items = {"Item 0", "Item 1", "Item 2"};

public:
  bool dropdown_clicked = false;

  sf::Vector2f position = sf::Vector2f(410, 40);

  bool is_mouse_over();
  std::string get_text() { return items.at(currently_selected_index); }
  std::string get_text(int index) { return items.at(index); }

  sf::Vector2f rect_size() { return largest_item_rect_size; };

  void clear_items() { items.clear(); }

  void add_items(std::initializer_list<std::string> options) {
    for (const auto &option : options) {
      items.push_back(option);
    }
    _compute_largest_item_rect_size();
  }

  void handle_inputs(sf::Event event);
  void Render();
};

#endif
