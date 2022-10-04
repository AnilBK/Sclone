#include "Container.hpp"

sf::Vector2f HBoxContainer::rect_size() {
  sf::Vector2f size{0.0f, 0.0f};

  for (const auto &child : children) {
    size.x += child->rect_size().x + padding.x;
    size.y = std::max(size.y, child->rect_size().y);
  }

  return size;
}

void HBoxContainer::reposition_children() {
  if (children.empty()) {
    return;
  }

  sf::Vector2f pos = position;

  for (auto &child : children) {
    child->setPosition(pos);
    pos.x += child->rect_size().x + padding.x;
  }
}

/*
void HBoxContainer::add_child(UIBaseClass &child) {
  sf::Vector2f pos{0.0f, 0.0f};
  if (children.size() > 0) {
    pos = children.at(0)->getPosition();
  }

  for (const auto &child : children) {
    pos.x += child->rect_size().x + padding.x;
  }

  children.push_back(&child);
  children.back()->setPosition(pos);
}
*/

/*
void HBoxContainer::add_child(UIBaseClass &child) {
 sf::Vector2f pos = position + sf::Vector2f(rect_size().x, 0.0f);

 children.push_back(&child);
 children.back()->setPosition(pos);
}
*/

/////////////////////////////////////////////
/////////////////////////////////////////////

sf::Vector2f VBoxContainer::rect_size() {
  sf::Vector2f size{0.0f, 0.0f};

  for (const auto &child : children) {
    size.x = std::max(size.x, child->rect_size().x);
    size.y += child->rect_size().y + padding.y;
  }

  return size;
}

void VBoxContainer::reposition_children() {
  if (children.empty()) {
    return;
  }

  sf::Vector2f pos = position;

  for (auto &child : children) {
    child->setPosition(pos);
    pos.y += child->rect_size().y + padding.y;
  }
}
