#ifndef GLOBALPROPERTYBINDINGS_HPP
#define GLOBALPROPERTYBINDINGS_HPP

#include "../Nodes/CircleShapeNode.hpp"

/// @brief This binds properties of all the nodes at once. Those properties
/// are freed onces this object leaves the scope. Only one instance of this
/// class should to be made. See 'PropertiesListUI' class as well.
class GlobalPropertyBindings {
public:
  GlobalPropertyBindings() { CircleShapeNode::bind(); }

  ~GlobalPropertyBindings() { delete CircleShapeNode::bounded_properties; }

  template <class T>
  static std::vector<typename T::PropertyVariant> *bounded_properties() {
    // We assume the properties are bounded to a vector named
    // 'bounded_properties' in the respective classes.
    return T::bounded_properties;
  }
};

#endif // GLOBALPROPERTYBINDINGS_HPP
