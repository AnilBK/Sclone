#include "RectangleShapeNode.hpp"

RectangleShapeNode::RectangleShapeNode(sf::RectangleShape p_rect_shape) {
  const auto rect_pos = p_rect_shape.getPosition();

  m_shape = p_rect_shape;
  m_shape.setPosition(sf::Vector2f());
  m_shape.setOrigin(m_shape.getSize().x / 2.0f, m_shape.getSize().y / 2.0f);

  setPosition(rect_pos);
}

void RectangleShapeNode::onDraw(sf::RenderTarget &target,
                                const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}
