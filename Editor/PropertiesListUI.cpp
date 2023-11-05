#include "PropertiesListUI.hpp"
#include "../Core/GlobalPropertyBindings.hpp"
#include "../Globals.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../UI/UILineInput.hpp"
#include "../Utils.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>

PropertiesListUI::PropertiesListUI(sf::Vector2f pos) {
  info_container.setPosition(pos);
}

template <class T>
void PropertiesListUI::apply_setter_fn(typename T::PropertyVariant p_property,
                                       HBoxContainer *hbox_ref) {
  // Use the setter function on the currently selected object with the value
  // in the UILineInput as the argument for the setter function.
  auto casted = dynamic_cast<T *>(target_object);
  if (!casted) {
    return;
  }

  std::visit(
      [this, hbox_ref, casted](auto &&property) {
        auto params_count = property.params_count;
        std::vector<float> results = get_values(hbox_ref, params_count);

        auto setter_fn = property.setter_fn.set;

        using PropertyType = std::decay_t<decltype(property)>;
        if constexpr (std::is_same_v<PropertyType, typename T::ColorType>) {
          auto color = sf::Color(results.at(0), results.at(1), results.at(2),
                                 results.at(3));
          setter_fn(*casted, color);
        } else if constexpr (std::is_same_v<PropertyType,
                                            typename T::FloatType>) {
          auto radius = results.at(0);
          setter_fn(*casted, radius);
        }
      },
      p_property);
}

template <class T>
void PropertiesListUI::add_fields(
    std::vector<PropertyField> &p_fields,
    const typename T::PropertyVariant &p_property) {

  auto h_box = std::make_shared<HBoxContainer>();
  auto hbox_ref = h_box.get();

  hbox_ref->padding.x = 10.0f;

  // In a horizontal row, whenever any of the input is set, then all the inputs
  // in that row call the same setter function.
  // This is done because the setter function expects 1 argument.
  // For example : setColor(sf::Color);
  // The setColor function requires sf::Color(4 floats) & we have to construct
  // this sf::Color from 4 individual items in that row.
  auto set_value_func = [this, p_property, hbox_ref]() {
    apply_setter_fn<T>(p_property, hbox_ref);
  };

  for (auto &[name, initial_value] : p_fields) {
    auto property_name_label = std::make_shared<UILabel>(name);

    auto property_input = std::make_shared<UILineInput>(initial_value);
    property_input->is_flat = false;

    property_input->enter_pressed_callback = set_value_func;

    hbox_ref->add_child(*property_name_label.get());
    hbox_ref->add_child(*property_input.get());

    property_ui_items.push_back(property_name_label);
    property_ui_items.push_back(property_input);
  }

  property_ui_items.push_back(h_box);
  info_container.add_child(*property_ui_items.back().get());
}

template <class T>
void PropertiesListUI::_add_property_to_property_list(
    typename T::PropertyVariant p_property) {

  std::visit(
      [this, p_property](auto &&property) {
        std::vector<PropertyField> fields;

        if (property.params == "" || property.params_count == 0 ||
            property.params_count == 1) {
          fields.push_back({property.name, "0"});
        } else {
          auto params = STRING_UTILITIES::SplitString(property.params, ',');
          for (auto &param : params) {
            fields.push_back({param, "0"});
          }
        }

        add_fields<T>(fields, p_property);
      },
      p_property);
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

void PropertiesListUI::fill_values(HBoxContainer *parent_hbox,
                                   const std::vector<std::string> &results) {
  for (std::size_t i = 0; i < results.size(); i++) {
    auto &child = parent_hbox->children.at((i * 2) + 1);

    if (auto line_input = dynamic_cast<UILineInput *>(child)) {
      // We are not currently setting custom values for this
      // property using UILineInput in the editor, so update it
      // using the getter fn.
      if (!line_input->line_input_active) {
        line_input->set_text(results[i]);
      }
    }
  }
}

std::vector<float> PropertiesListUI::get_values(HBoxContainer *parent_hbox,
                                                std::size_t count) {
  std::vector<float> results;
  results.reserve(count);

  for (std::size_t i = 0; i < count; i++) {
    // First index : UILabel.
    // Second index : UILineInput.
    auto &child = parent_hbox->children.at((i * 2) + 1);

    if (auto line_input = dynamic_cast<UILineInput *>(child)) {
      auto param =
          MATH_UTILITIES::str_to_float(line_input->get_text_no_prefix(), 1.0f);
      results.push_back(param);
    }
  }

  return results;
}

template <class T> void PropertiesListUI::_update_property_list_ui() {
  if (property_ui_items.empty()) {
    return;
  }

  auto casted_obj = dynamic_cast<T *>(target_object);
  if (!casted_obj) {
    return;
  }

  auto bounded_properties_ref = GlobalPropertyBindings::bounded_properties<T>();

  std::size_t count = 0;
  for (auto &bound_property : *bounded_properties_ref) {
    auto ui_item = info_container.children.at(count);

    if (auto parent_hbox = dynamic_cast<HBoxContainer *>(ui_item)) {

      std::visit(
          [&](auto &&property) {
            auto value = property.getter_fn.get(*casted_obj);

            std::vector<std::string> obtained_values;

            using PropertyType = std::decay_t<decltype(property)>;
            if constexpr (std::is_same_v<PropertyType, typename T::ColorType>) {
              obtained_values = {
                  std::to_string(value.r), std::to_string(value.g),
                  std::to_string(value.b), std::to_string(value.a)};
            } else if constexpr (std::is_same_v<PropertyType,
                                                typename T::FloatType>) {
              obtained_values = {std::to_string(value)};
            }

            auto params_count = property.params_count;

            if (params_count == obtained_values.size()) {
              fill_values(parent_hbox, obtained_values);
            }
          },
          bound_property);
    }

    count++;
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