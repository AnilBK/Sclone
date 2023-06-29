#include "UILabel.hpp"

void UILabel::setPosition(sf::Vector2f pos) {
  text.setPosition(pos);
  update_internal();
}

sf::Vector2f UILabel::get_actual_text_position() {
  const auto bounds = get_rectangle().getGlobalBounds();
  return bounds.getPosition();
}

sf::Vector2f UILabel::get_actual_text_size() {
  const auto bounds = get_rectangle().getGlobalBounds();
  return bounds.getSize();
}

sf::Vector2f UILabel::getPosition() { return text.getPosition(); }

void UILabel::set_text(const std::string &str) {
  text.setString(str);
  update_internal();
}

std::string UILabel::get_text() { return text.getString(); }

sf::Vector2f UILabel::rect_size() {
  sf::FloatRect textBounds = text.getGlobalBounds();
  return textBounds.getSize();
}

void UILabel::update_internal() {
  // This provides an actual way to get the rect covering the actual text
  // object.
  // We cache that in the outline rectangle, so we can access it from outside.
  sf::FloatRect textBounds = text.getGlobalBounds();
  outline.setOrigin(text.getOrigin());
  outline.setPosition(textBounds.getPosition());
  outline.setSize(textBounds.getSize());
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

void UILabel::RenderDebug() {
  /*
  sf::RectangleShape debug_shape;
  debug_shape.setPosition(get_actual_text_position());
  debug_shape.setSize(get_actual_text_size());
  debug_shape.setFillColor(sf::Color(255, 0, 0, 200));
  window.draw(debug_shape);
  */
  window.draw(outline);
}

std::size_t UILabel::max_character_size(sf::Text &p_text) {
  std::string copy = p_text.getString();
  p_text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  size_t CharacterSize = p_text.getCharacterSize();
  auto Font = p_text.getFont();
  std::string String = p_text.getString().toAnsiString();
  bool bold = (p_text.getStyle() & sf::Text::Bold);

  std::size_t max_height = 0;
  for (size_t x = 0; x < p_text.getString().getSize(); ++x) {
    sf::Uint32 Character = String.at(x);

    const auto &CurrentGlyph = Font->getGlyph(Character, CharacterSize, bold);

    size_t height = CurrentGlyph.bounds.height;
    max_height = std::max(max_height, height);
  }

  // Reset back the text to original.
  p_text.setString(copy);
  return max_height;
}
