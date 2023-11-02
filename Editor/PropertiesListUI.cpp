#include "PropertiesListUI.hpp"
#include "../Core/GlobalPropertyBindings.hpp"
#include "../Globals.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../UI/UILineInput.hpp"
#include "../Utils.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

PropertiesListUI::PropertiesListUI(sf::Vector2f pos) {
  info_container.setPosition(pos);
}

template <class T>
void PropertiesListUI::apply_setter_fn(typename T::setter_fn_type p_setter_fn,
                                       UILineInput *line_input_ref) {

  // Use the setter function on the currently selected object with the value
  // in the UILineInput as the argument for the setter function.

  if (auto casted = dynamic_cast<T *>(target_object)) {
    auto param = MATH_UTILITIES::str_to_float(
        line_input_ref->get_text_no_prefix(), 1.0f);

    p_setter_fn(*casted, param);
  } else {
    ERR_CRASH_IF(true, "Setting up value for member variable failed.")
  }
}

template <class T>
void PropertiesListUI::_add_property_to_property_list(
    typename T::Property property) {

  auto property_name_label = std::make_shared<UILabel>(property.property_name);

  auto property_input = std::make_shared<UILineInput>("0");
  property_input->is_flat = false;

  auto setter_fn = property.setter_fn;
  auto line_input_ref = property_input.get();
  auto set_value_func = [this, setter_fn, line_input_ref]() {
    apply_setter_fn<T>(setter_fn, line_input_ref);
  };
  property_input->enter_pressed_callback = set_value_func;

  property_ui_items.push_back(property_name_label);
  info_container.add_child(*property_ui_items.back().get());

  property_ui_items.push_back(property_input);
  info_container.add_child(*property_ui_items.back().get());
}

void PropertiesListUI::build_initial_property_list_ui(Node *p_target_object) {
  target_object = p_target_object;

  // First Clear all previous properties.
  info_container.children.clear();
  property_ui_items.clear();

  // Fetch all the properties for this node that has been bounded.
  // And build them in a VBoxContainer to show up in the editor.
  // This could be like 'radius' property for the 'CircleShapeNode'.

#define BUILD_PROPERTY_LIST(T)                                                 \
  for (auto &property : *GlobalPropertyBindings::bounded_properties<T>()) {    \
    _add_property_to_property_list<T>(property);                               \
  }

#define OBJECT_IS(T) auto casted = dynamic_cast<T *>(p_target_object)
  if (OBJECT_IS(CircleShapeNode)) {
    BUILD_PROPERTY_LIST(CircleShapeNode)
  }
#undef OBJECT_IS

#undef BUILD_PROPERTY_LIST
}

template <class T> void PropertiesListUI::_update_property_list_ui() {
  if (property_ui_items.empty()) {
    return;
  }

  auto bounded_properties_ref = GlobalPropertyBindings::bounded_properties<T>();

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

    if (auto line_input = dynamic_cast<UILineInput *>(ui_item)) {
      // We are not currently setting custom values for this property using
      // UILineInput in the editor, so update it using the getter fn.
      if (!line_input->line_input_active) {
        if (auto casted_obj = dynamic_cast<T *>(target_object)) {
          auto value = property.getter_fn(*casted_obj);

          // Format the number with 2 digits precision.
          std::ostringstream oss;
          oss << std::fixed << std::setprecision(2) << value;
          const std::string &result = oss.str();

          line_input->set_text(result);
        }
      }
    }

    // Since, every property has 2 UI Items associated with it.
    count += 2;
  }
}

void PropertiesListUI::Update() {
#define OBJECT_IS(T) auto current_node = dynamic_cast<T *>(target_object)

  if (OBJECT_IS(CircleShapeNode)) {
    _update_property_list_ui<CircleShapeNode>();
  }

#undef OBJECT_IS
}

void PropertiesListUI::handle_inputs(sf::Event event) {
  info_container.handle_inputs(event);
}

void PropertiesListUI::Render() { info_container.Render(); }