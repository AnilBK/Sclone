#ifndef PROPERTIESLISTUI_HPP
#define PROPERTIESLISTUI_HPP

#include "../Nodes/Node.hpp"
#include "../UI/Container.hpp"
#include <memory>
#include <string>
#include <vector>

/// @brief A VBoxContainer that displays the various properties bounded to a
/// node. It handles creation & update of that list.
class PropertiesListUI {
public:
  PropertiesListUI(sf::Vector2f pos);

  /// @brief Clear all previous properties displayed in the editor. Fetch
  /// properties bounded to 'p_target_object' and create a UI List.
  /// @param p_target_object The node whose properties are to be fetched and
  /// drawn.
  void build_initial_property_list_ui(Node *p_target_object);

  void handle_inputs(sf::Event event);

  /// @brief Fetch the actual values for the bounded properties of
  /// 'p_target_object' and update in the container to show those actual values.
  /// @param p_target_object The node whose properties are to be fetched and
  /// drawn.
  void Update(Node *p_target_object);

  void Render();

private:
  VBoxContainer info_container;

  // When properties are fetched, we need to display them in UI items. Those
  // created UI items are stored here and the reference to those UI items are
  // pushed to 'info_container'.
  std::vector<std::shared_ptr<UIBaseClass>> property_ui_items;

  template <class NodeType>
  void apply_setter_fn(std::size_t ui_item_index, NodeType *p_target_object,
                       typename NodeType::setter_fn_type p_fn_setter);

  void _add_property_to_property_list(const std::string &property_name);

  template <class NodeType>
  void _update_property_list_ui(NodeType *p_target_object);
};

#endif // PROPERTIESLISTUI_HPP
