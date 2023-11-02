#ifndef CIRCLE_SHAPE_NODE_HPP
#define CIRCLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>

class CircleShapeNode : public Node {
public:
  CircleShapeNode(sf::CircleShape p_circle_shape);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::FloatRect get_global_bounds() override;

  sf::CircleShape &get_shape() { return m_shape; }

  void update_shape_bounds();

  void set_radius(float p_new_radius);

  //////////////////////////////////////////////////////////////////////
  //                      Bindings Related Code.                      //
  //////////////////////////////////////////////////////////////////////
  using fn_type = std::function<float(CircleShapeNode &circle)>;

  struct Property {
    std::string property_name;
    fn_type fn;
  };

  // Important : Always delete them.
  // Deletion is Handled by '_global_binder'.
  inline static std::vector<Property> *bounded_properties =
      new std::vector<Property>();

  static void bind();

private:
  sf::CircleShape m_shape;
};

#endif // CIRCLE_SHAPE_NODE_HPP
