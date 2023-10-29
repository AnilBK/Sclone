#ifndef CIRCLE_SHAPE_NODE_HPP
#define CIRCLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class CircleShapeNode : public Node {
public:
  CircleShapeNode(sf::CircleShape p_circle_shape);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::FloatRect get_global_bounds() override;

  sf::CircleShape &get_shape() { return m_shape; }

private:
  sf::CircleShape m_shape;
};

#endif // CIRCLE_SHAPE_NODE_HPP
