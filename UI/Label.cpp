#include "Label.hpp"

void Label::setText(std::string txt) { label.setString(txt); }

std::string Label::getText() { return label.getString(); }

sf::Vector2f Label::getPosition() {
  return label.getPosition();
  // Maybe this:
  // return {label.getGlobalBounds().left, label.getGlobalBounds().top};
}

sf::Vector2f Label::getRectPosition() {
  const auto bounds = label.getGlobalBounds();
  return {bounds.left, bounds.top};
}

void Label::setPosition(sf::Vector2f pos) { label.setPosition(pos); }

sf::Vector2f Label::rect_size() {
  auto bounds = label.getGlobalBounds();
  return {bounds.width, bounds.height};
}

void Label::RenderDebug() {
  sf::RectangleShape debug_shape;
  debug_shape.setPosition(getRectPosition());
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
}

void Label::Render() {
  // RenderDebug();
  window.draw(label);
}