#ifndef UI_DROPDOWN
#define UI_DROPDOWN

#include "Container.hpp"
#include "UIBaseClass.hpp"
#include "UIButton.hpp"
#include "UILabel.hpp"
#include <initializer_list>
#include <memory>

class UIDropDown : public UIBaseClass {
private:
  std::size_t currently_selected_index = 0;

  sf::Vector2f position = sf::Vector2f(410, 40);

  // @brief The rect size required for the largest item in the list. Every
  // button in the drop down have this same size.
  sf::Vector2f largest_item_rect_size;

  bool callbacks_registered = false;

  /// @brief The single button at the base. Clicking this expands the DropDown.
  UIButton base_button = UIButton("");

  bool expanded = false;

  /// @brief All the items in the list.
  std::vector<std::shared_ptr<UIButton>> list_item_buttons;

  VBoxContainer drop_down_vbox;

  /// @brief Add a single item name to the list.
  /// @param str The string to add to the list.
  void _add_item(const std::string &str);

  void _add_items(const std::initializer_list<std::string> options);

  void register_callbacks();

  void _select_item(std::size_t idx) {
    currently_selected_index = idx;
    base_button.set_text(get_text());
  }

  void toggle_fold() { expanded = !expanded; }

  void select_item_and_fold(std::size_t idx) {
    _select_item(idx);
    toggle_fold();
  }

  /// @brief The base DropDown button when the DropDown isn't expanded.
  void _draw_base_button();

  /// @brief Draws all the items in the list that the user can pick.
  void _draw_whole_list();

public:
  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void handle_inputs(sf::Event event) override;
  void Render() override;

  /// @brief Text of the currently selected item.
  /// @return String containing the text.
  std::string get_text() {
    return list_item_buttons.at(currently_selected_index)->get_text();
  }

  UIDropDown(std::initializer_list<std::string> options) {
    _add_items(options);
    _select_item(0);

    base_button.left_padding = 0.0F;
    base_button.set_text_align(TEXT_ALIGN::LEFT);

    drop_down_vbox.padding.y = 0.0F;
  }
};

#endif
