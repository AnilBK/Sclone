#ifndef GLOBALPROPERTYBINDINGS_HPP
#define GLOBALPROPERTYBINDINGS_HPP

#include "../Nodes/CircleShapeNode.hpp"
#include "../Nodes/RectangleShapeNode.hpp"

/// @brief This binds properties of all the nodes at once. Those properties
/// are freed onces this object leaves the scope. Only one instance of this
/// class should to be made. See 'PropertiesListUI' class as well.
class GlobalPropertyBindings {
public:
  GlobalPropertyBindings() {
    CircleShapeNode::bind();
    RectangleShapeNode::bind();
  }

  template <class T>
  static std::vector<typename T::PropertyVariant> *bounded_properties() {
    // Return address of the statically allocated vector
    return &T::bounded_properties;
  }
};

#endif // GLOBALPROPERTYBINDINGS_HPP
