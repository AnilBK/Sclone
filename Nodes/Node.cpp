#include "Node.hpp"

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