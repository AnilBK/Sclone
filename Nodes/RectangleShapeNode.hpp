#ifndef RECTANGLE_SHAPE_NODE_HPP
#define RECTANGLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class RectangleShapeNode : public Node {
public:
  RectangleShapeNode(sf::RectangleShape p_rect_shape);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::RectangleShape &get_shape() { return m_shape; }

private:
  sf::RectangleShape m_shape;
};

#endif // RECTANGLE_SHAPE_NODE_HPP
