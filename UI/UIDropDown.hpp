#ifndef UI_DROPDOWN
#define UI_DROPDOWN

#include "UIBaseClass.hpp"
#include "UILabel.hpp"
#include <initializer_list>
#include <memory>

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

  /// @brief Computes the maximum size that fits all the items in the list. All
  /// the items in the list are then fit into this size while rendering.
  void _compute_maximum_size_for_items();

  /// @brief The single label at the base. Clicking this expands the DropDown.
  UILabel base_label;

  /// @brief All the items in the list.
  std::vector<std::shared_ptr<UILabel>> items;

public:
  bool dropdown_clicked = false;

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void handle_inputs(sf::Event event) override;
  void Render() override;

  /// @brief Text of the currently selected item.
  /// @return String containing the text.
  std::string get_text() {
    return items.at(currently_selected_index)->get_text();
  }

  void select_item(std::size_t idx) {
    currently_selected_index = idx;
    base_label.set_text(get_text());
  }

  /// @brief Draw a single item with given index.
  /// @param idx Index of the item to draw.
  void draw_item(std::size_t idx);

  /// @brief Add a single item name to the list.
  /// @param str The string to add to the list.
  void add_item(const std::string &str);

  void add_items(const std::initializer_list<std::string> options);

  UIDropDown(std::initializer_list<std::string> options) {
    add_items(options);
    select_item(0);
    _compute_maximum_size_for_items();
  }
};

#endif
