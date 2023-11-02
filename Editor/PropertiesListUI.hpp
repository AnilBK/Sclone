#ifndef PROPERTIESLISTUI_HPP
#define PROPERTIESLISTUI_HPP

#include "../Nodes/Node.hpp"
#include "../UI/Container.hpp"
#include "../UI/UILineInput.hpp"
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
  void Update();

  void Render();

private:
  Node *target_object;

  VBoxContainer info_container;

  // When properties are fetched, we need to display them in UI items. Those
  // created UI items are stored here and the reference to those UI items are
  // pushed to 'info_container'.
  std::vector<std::shared_ptr<UIBaseClass>> property_ui_items;

  template <class NodeType>
  void apply_setter_fn(typename NodeType::setter_fn_type p_setter_fn,
                       UILineInput *line_input_ref);

  template <class NodeType>
  void _add_property_to_property_list(typename NodeType::Property property);

  template <class NodeType> void _update_property_list_ui();
};

#endif // PROPERTIESLISTUI_HPP
