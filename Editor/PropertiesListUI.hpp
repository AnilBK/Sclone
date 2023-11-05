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

  /// @brief Suppose when we bind a float variable named "Radius", we create a
  /// Label named "Radius" & a LineInput with the initial value. This two UI
  /// items are combinely called a Property Field.
  struct PropertyField {
    std::string name, initial_value;
  };

  template <class T>
  void apply_setter_fn(typename T::PropertyVariant p_property,
                       HBoxContainer *hbox_ref);

  template <class T>
  void _add_property_to_property_list(typename T::PropertyVariant p_property);

  template <class T>
  void add_fields(std::vector<PropertyField> &p_fields,
                  const typename T::PropertyVariant &p_property);

  template <class T> void _update_property_list_ui();

  /// @brief Fill a row with the values.
  /// @param parent_hbox The reference to the parent HBox which contains line
  /// inputs which are to be filled.
  /// @param results Values which are to be filled in this row.
  void fill_values(HBoxContainer *parent_hbox,
                   const std::vector<std::string> &results);

  /// @brief Get 'count' number of values that are in this row.
  /// @param parent_hbox The reference to the parent HBox for a given row which
  /// contains different line inputs whose values are to be fetched.
  /// @param count Number of values to be fetched from the provided row.
  /// @return A list of values.
  std::vector<float> get_values(HBoxContainer *parent_hbox, std::size_t count);
};

#endif // PROPERTIESLISTUI_HPP
