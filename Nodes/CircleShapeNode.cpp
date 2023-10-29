#include "CircleShapeNode.hpp"

CircleShapeNode::CircleShapeNode(sf::CircleShape p_circle_shape) {
  const auto circle_pos = p_circle_shape.getPosition();

  m_shape = p_circle_shape;
  m_shape.setPosition(sf::Vector2f());
  m_shape.setOrigin(m_shape.getGlobalBounds().getSize().x / 2.0f,
                    m_shape.getGlobalBounds().getSize().y / 2.0f);

  setPosition(circle_pos);
}

sf::FloatRect CircleShapeNode::get_global_bounds() {
  return m_shape.getGlobalBounds();
}

void CircleShapeNode::onDraw(sf::RenderTarget &target,
                             const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}