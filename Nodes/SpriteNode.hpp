#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>

class SpriteNode : public Node {
public:
  SpriteNode(sf::Sprite p_sprite);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::FloatRect get_global_bounds() override;

  sf::Sprite &get_shape() { return m_shape; }

private:
  sf::Sprite m_shape;
};

#endif // SPRITENODE_HPP
