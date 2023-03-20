#ifndef UI_DROPDOWN
#define UI_DROPDOWN

#include "UIBaseClass.hpp"
#include <initializer_list>

class UIDropDown : public UIBaseClass {
private:
  int currently_selected_index = 0;
  bool mouse_over = false;
  int mouse_over_index = -1;

  sf::Vector2f position = sf::Vector2f(410, 40);

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

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void handle_inputs(sf::Event event) override;
  void Render() override;

  std::string get_text() { return items.at(currently_selected_index); }
  std::string get_text(int index) { return items.at(index); }

  void clear_items() { items.clear(); }
  void add_items(std::initializer_list<std::string> options) {
    for (const auto &option : options) {
      items.push_back(option);
    }
    _compute_largest_item_rect_size();
  }
};

#endif
