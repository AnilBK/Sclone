#ifndef UITREEVIEW_H
#define UITREEVIEW_H

#include "../Globals.hpp"
#include "Button.hpp"
#include "Container.hpp"
#include <functional>
#include <memory>
#include <optional>

class UITreeView {
  int global_id = 0;

  struct tree_item {
    int id;
    std::shared_ptr<Button> item;
  };

  VBoxContainer list_parent;
  VBoxContainer list_vbox;

  sf::FloatRect list_world;
  sf::View list_view;
  sf::RectangleShape list_bg;

  std::vector<tree_item> buttons;

public:
  UITreeView(sf::Vector2f p_pos, sf::Vector2f p_size);

  void init_ui();

  void update_item_name(int index, std::string p_new_name);

  void select_item_by_index(int p_idx);

  std::optional<int> index_of_item_with_id(int p_id);

  /// @brief Adds item to the tree.
  /// @param p_text Name for the new item in the tree.
  /// @param clicked_callback Callback when the tree item is pressed.
  /// @return Unique id of the currently added tree item.
  [[nodiscard]] int add_item(const std::string &p_text,
                             std::function<void()> clicked_callback);

  void handle_inputs(sf::Event event);

  void Render();
};

#endif // UITREEVIEW_H
