#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "../Globals.hpp"
#include "UIBaseClass.hpp"
#include <vector>

class Container : public UIBaseClass {
public:
  sf::Vector2f padding{0.0f, 0.0f};
  sf::Vector2f position;
  std::vector<UIBaseClass *> children;

  virtual void reposition_children() = 0;

  sf::Vector2f getPosition() override { return position; };

  void setPosition(sf::Vector2f pos) override {
    position = pos;
    reposition_children();
  }

  sf::Vector2f rect_size() override { return sf::Vector2f(); };

  void Render() override {
    for (auto &child : children) {
      child->Render();
    }
  }

  void add_child(UIBaseClass &child) {
    child.connect("resized", [this]() { this->reposition_children(); });
    children.push_back(&child);
    reposition_children();
  }

  void handle_inputs(sf::Event event) override {
    for (auto &child : children) {
      child->handle_inputs(event);
    }
  }
};

class HBoxContainer : public Container {
public:
  HBoxContainer() { padding = {25.0f, 0.0f}; }

  void reposition_children() override;
  sf::Vector2f rect_size() override;
};

class VBoxContainer : public Container {
public:
  VBoxContainer() { padding = {0.0f, 25.0f}; }

  void reposition_children() override;
  sf::Vector2f rect_size() override;
};

#endif