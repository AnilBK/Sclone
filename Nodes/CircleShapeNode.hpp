#ifndef CIRCLE_SHAPE_NODE_HPP
#define CIRCLE_SHAPE_NODE_HPP

#include "../Core/GenerateBindingsCode.hpp"
#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <variant>

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
  GENERATE_BINDINGS_CODE(CircleShapeNode)

private:
  sf::CircleShape m_shape;
};

#endif // CIRCLE_SHAPE_NODE_HPP
