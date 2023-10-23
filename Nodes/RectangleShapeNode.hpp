#ifndef RECTANGLE_SHAPE_NODE_HPP
#define RECTANGLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class RectangleShapeNode : public Node {
public:
  RectangleShapeNode(sf::Vector2f pos = sf::Vector2f());

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

private:
  sf::RectangleShape m_shape;
};

#endif // RECTANGLE_SHAPE_NODE_HPP
