#include "UILabel.hpp"

void UILabel::setPosition(sf::Vector2f pos) {
  text.setPosition(pos);
  update_internal();
}

sf::Vector2f UILabel::getPosition() { return text.getPosition(); }

void UILabel::set_text(const std::string &str) { text.setString(str); }

sf::Vector2f UILabel::rect_size() {
  sf::FloatRect textBounds = text.getGlobalBounds();
  return sf::Vector2f(textBounds.width, textBounds.height);
}

void UILabel::update_internal() {
  // This provides an actual way to get the rect covering the actual text
  // object.
  // We cache that in the outline rectangle, so we can access it from outside.
  sf::FloatRect textBounds = text.getGlobalBounds();
  outline.setOrigin(text.getOrigin());
  outline.setPosition({textBounds.left, textBounds.top});
  outline.setSize(sf::Vector2f(textBounds.width, textBounds.height));
}

UILabel::UILabel(const std::string &str, const sf::Vector2f pos) {
  LabelFont.loadFromFile("OpenSans-Regular.ttf");

  text.setString(str);
  text.setFont(LabelFont);
  text.setCharacterSize(DEFAULT_TEXT_FONT_SIZE);
  text.setFillColor(sf::Color::Black);

  setPosition(pos);

  outline.setFillColor(sf::Color::Transparent);
  outline.setOutlineThickness(2);
  outline.setOutlineColor(sf::Color::Red);
}

void UILabel::Render() {
  // window.draw(outline);
  window.draw(text);
}