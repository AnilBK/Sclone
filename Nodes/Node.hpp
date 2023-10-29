#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Node : public sf::Drawable, public sf::Transformable {
public:
  void addChild(Node *n);

  virtual sf::FloatRect getGlobalBounds();

  virtual sf::FloatRect get_global_bounds();

  virtual void onDraw(sf::RenderTarget &target,
                      const sf::Transform &transform) const {}

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
  std::vector<Node *> m_children;
};

#endif // NODE_HPP
