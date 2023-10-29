#include "SpriteNode.hpp"

SpriteNode::SpriteNode(sf::Sprite p_sprite) {
  const auto sprite_pos = p_sprite.getPosition();

  m_shape = p_sprite;
  // Set the position of the parent node to the position of the sprite provided.
  // But set the inner sprites position to (0,0).
  m_shape.setPosition(sf::Vector2f());
  setPosition(sprite_pos);
}

sf::FloatRect SpriteNode::get_global_bounds() {
  return m_shape.getGlobalBounds();
}

void SpriteNode::onDraw(sf::RenderTarget &target,
                        const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}
