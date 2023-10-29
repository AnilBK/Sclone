#include "Node.hpp"

sf::FloatRect Node::getGlobalBounds() {
  auto pos = getPosition();
  auto bounds = get_global_bounds();

  // The way our node works, we have to do this to get the actual global bounds
  // of the node.
  bounds.left += pos.x;
  bounds.top += pos.y;

  return bounds;
}

sf::FloatRect Node::get_global_bounds() { return sf::FloatRect(); }

void Node::addChild(Node *n) {
  //   n->setOrigin(getPosition());
  m_children.push_back(n);
}

void Node::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

  onDraw(target, states.transform);

  for (auto &child : m_children) {
    child->draw(target, states.transform);
  }
}