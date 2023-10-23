#include "CircleShapeNode.hpp"

CircleShapeNode::CircleShapeNode(sf::Vector2f pos) {
  m_shape = sf::CircleShape(300.f);
  m_shape.setPosition(sf::Vector2f());
  m_shape.setFillColor(sf::Color::Green);
  m_shape.setRadius(30.0f);

  setPosition(pos);
}

void CircleShapeNode::onDraw(sf::RenderTarget &target,
                             const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}