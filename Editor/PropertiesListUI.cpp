#include "PropertiesListUI.hpp"
#include "../Core/GlobalPropertyBindings.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../UI/UILabel.hpp"

PropertiesListUI::PropertiesListUI(sf::Vector2f pos) {
  info_container.setPosition(pos);
}

void PropertiesListUI::_add_property_to_property_list(
    const std::string &property_name) {
  auto radius_label = std::make_shared<UILabel>(property_name);
  property_ui_items.push_back(radius_label);

  info_container.add_child(*property_ui_items.back().get());
}

void PropertiesListUI::build_initial_property_list_ui(Node *p_target_object) {
  // First Clear all previous properties.
  info_container.children.clear();
  property_ui_items.clear();

  // Fetch all the properties for this node that has been bounded.
  // And build them in a VBoxContainer to show up in the editor.
  // This could be like 'radius' property for the 'CircleShapeNode'.

#define BUILD_PROPERTY_LIST(T)                                                 \
  for (auto &property : *GlobalPropertyBindings::bounded_properties<T>()) {    \
    _add_property_to_property_list(property.property_name);                    \
  }

#define OBJECT_IS(T) auto casted = dynamic_cast<T *>(p_target_object)
  if (OBJECT_IS(CircleShapeNode)) {
    BUILD_PROPERTY_LIST(CircleShapeNode)
  }
#undef OBJECT_IS

#undef BUILD_PROPERTY_LIST
}

template <class NodeType>
void PropertiesListUI::_update_property_list_ui(NodeType *p_target_object) {
  if (property_ui_items.empty()) {
    return;
  }

  auto bounded_properties_ref =
      GlobalPropertyBindings::bounded_properties<NodeType>();

  if (property_ui_items.size() != bounded_properties_ref->size()) {
    return;
  }

  std::size_t count = 0;

  for (auto &property : *bounded_properties_ref) {
    auto ui_item = property_ui_items.at(count).get();

    if (auto label = dynamic_cast<UILabel *>(ui_item)) {
      auto value = property.fn(*p_target_object);
      label->set_text(property.property_name + " : " + std::to_string(value));
    }

    count++;
  }
}

void PropertiesListUI::Update(Node *p_target_object) {
#define OBJECT_IS(T) auto current_node = dynamic_cast<T *>(p_target_object)

  if (OBJECT_IS(CircleShapeNode)) {
    _update_property_list_ui(current_node);
  }

#undef OBJECT_IS
}

void PropertiesListUI::handle_inputs(sf::Event event) {
  info_container.handle_inputs(event);
}

void PropertiesListUI::Render() { info_container.Render(); }