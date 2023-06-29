#include "UISprite.hpp"

sf::Vector2f UISprite::getPosition() { return sprite.getPosition(); }

void UISprite::setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }

sf::Vector2f UISprite::rect_size() {
  auto bounds = sprite.getGlobalBounds();
  return bounds.getSize();
}

void UISprite::RenderDebug() {
  auto bounds = sprite.getGlobalBounds();
  sf::Vector2f pos = bounds.getPosition();

  sf::RectangleShape debug_shape;
  debug_shape.setPosition(pos);
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
}

void UISprite::Render() {
  // RenderDebug();
  window.draw(sprite);
}