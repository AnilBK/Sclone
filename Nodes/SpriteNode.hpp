#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class SpriteNode : public Node {
public:
  SpriteNode(sf::Vector2f pos = sf::Vector2f());

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

private:
  sf::CircleShape m_shape;
};

#endif // SPRITENODE_HPP
