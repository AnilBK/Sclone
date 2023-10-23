#include "RectangleShapeNode.hpp"

RectangleShapeNode::RectangleShapeNode(sf::Vector2f pos) {
  m_shape.setPosition(pos);
  m_shape.setFillColor(sf::Color::Black);
  m_shape.setSize({200, 200});
}

void RectangleShapeNode::onDraw(sf::RenderTarget &target,
                                const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}
