#ifndef CIRCLE_SHAPE_NODE_HPP
#define CIRCLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class CircleShapeNode : public Node {
public:
  CircleShapeNode(sf::Vector2f pos = sf::Vector2f());

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

private:
  sf::CircleShape m_shape;
};

#endif // CIRCLE_SHAPE_NODE_HPP
