#include "PropertiesListUI.hpp"
#include "../Core/GlobalPropertyBindings.hpp"
#include "../Globals.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../UI/UILineInput.hpp"
#include "../Utils.hpp"
#include <iostream>

PropertiesListUI::PropertiesListUI(sf::Vector2f pos) {
  info_container.setPosition(pos);
}

void PropertiesListUI::_add_property_to_property_list(
    const std::string &property_name) {

  auto property_name_label = std::make_shared<UILabel>(property_name);

  auto property_input = std::make_shared<UILineInput>(property_name);
  property_input->is_flat = false;
  // TODO ?? Setup Enter pressed callback here.
  // As of now, we have done in update loop.

  property_ui_items.push_back(property_name_label);
  info_container.add_child(*property_ui_items.back().get());

  property_ui_items.push_back(property_input);
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
void PropertiesListUI::apply_setter_fn(
    std::size_t ui_item_index, NodeType *p_target_object,
    typename NodeType::setter_fn_type p_fn_setter) {

  auto ui_item = property_ui_items.at(ui_item_index).get();

  if (auto label = dynamic_cast<UILineInput *>(ui_item)) {
    auto val = MATH_UTILITIES::str_to_float(label->get_text_no_prefix(), 1.0f);
    p_fn_setter(*p_target_object, val);
  } else {
    ERR_CRASH_IF(true, "Setting up value for member variable failed.")
  }
}

template <class NodeType>
void PropertiesListUI::_update_property_list_ui(NodeType *p_target_object) {
  if (property_ui_items.empty()) {
    return;
  }

  auto bounded_properties_ref =
      GlobalPropertyBindings::bounded_properties<NodeType>();

  // Every property has 2 UI Items related.
  if (property_ui_items.size() != bounded_properties_ref->size() * 2) {
    return;
  }

  // Every Property has the following UI Items.
  // ---- Index (0) : Property Label (property_name_label)
  // ---- Index (1) : Property Input (property_input)

  // So, we start from index 1.
  std::size_t count = 1;

  for (auto &property : *bounded_properties_ref) {
    auto ui_item = property_ui_items.at(count).get();

    if (auto label = dynamic_cast<UILineInput *>(ui_item)) {
      // We are not currently setting custom values for this property using
      // UILineInput in the editor, so update it using the getter fn.
      if (!label->line_input_active) {
        auto value = std::to_string(property.getter_fn(*p_target_object));
        label->set_text(value);
      }

      // No callbacks set up.
      // Setup the callbacks here so that we get update reference to
      // 'p_target_object'. But we could cache it, since it doesnt change ??
      // TODO ??
      if (!label->enter_pressed_callback) {
        auto setter_fn = property.setter_fn;

        auto set_value = [this, count, p_target_object, setter_fn]() {
          apply_setter_fn(count, p_target_object, setter_fn);
        };

        label->enter_pressed_callback = set_value;
      }
    }

    // Since, every property has 2 UI Items associated with it.
    count += 2;
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