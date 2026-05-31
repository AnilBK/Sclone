#include "UILabel.hpp"
#include <unordered_map>

sf::FloatRect UILabel::get_consistent_local_bounds(const sf::Text &p_text) {
  // top/height from the dummy string depend only on the font and character
  // size, never on the actual string content, so we cache those two metrics.
  // The cache is a function-local static(safe for single-threaded SFML use).

  struct CacheKey {
    const sf::Font *font;
    unsigned int charSize;
    bool operator==(const CacheKey &o) const noexcept {
      return font == o.font && charSize == o.charSize;
    }
  };

  struct CacheKeyHash {
    std::size_t operator()(const CacheKey &k) const noexcept {
      std::size_t h = std::hash<const sf::Font *>{}(k.font);
      // boost-style hash_combine
      h ^= std::hash<unsigned int>{}(k.charSize) + 0x9e3779b9u + (h << 6) +
           (h >> 2);
      return h;
    }
  };

  struct Metrics {
    float top, height;
  };

  static std::unordered_map<CacheKey, Metrics, CacheKeyHash> s_cache;

  const CacheKey key{p_text.getFont(), p_text.getCharacterSize()};

  auto it = s_cache.find(key);
  if (it == s_cache.end()) {
    // Cache miss.
    sf::Text dummy = p_text;
    dummy.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                    "0123456789!@#$%^&*()-=_+[]\\{}|;':\",./<>?");
    const sf::FloatRect db = dummy.getLocalBounds();
    it = s_cache.emplace(key, Metrics{db.top, db.height}).first;
  }

  // real left/width
  sf::FloatRect bounds = p_text.getLocalBounds();
  // stable font metrics
  bounds.top = it->second.top;
  bounds.height = it->second.height;

  if (p_text.getString().isEmpty()) {
    bounds.width = 0.0f;
    bounds.left = 0.0f;
  }
  return bounds;
}

void UILabel::setPosition(sf::Vector2f pos) {
  text.setPosition(pos);
  update_internal();
}

sf::Vector2f UILabel::get_actual_text_position() {
  return text.getTransform()
      .transformRect(get_consistent_local_bounds(text))
      .getPosition();
}

sf::Vector2f UILabel::get_actual_text_size() {
  return text.getTransform()
      .transformRect(get_consistent_local_bounds(text))
      .getSize();
}

sf::Vector2f UILabel::getPosition() { return text.getPosition(); }

void UILabel::set_text(const std::string &str) {
  text.setString(str);
  update_internal();
}

std::string UILabel::get_text() { return text.getString(); }

sf::Vector2f UILabel::rect_size() {
  return text.getTransform()
      .transformRect(get_consistent_local_bounds(text))
      .getSize();
}

void UILabel::update_internal() {
  const sf::FloatRect textBounds =
      text.getTransform().transformRect(get_consistent_local_bounds(text));
  outline.setOrigin(0.0f, 0.0f);
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

void UILabel::Render() { window.draw(text); }

void UILabel::RenderDebug() { window.draw(outline); }

std::size_t UILabel::max_character_size(sf::Text &p_text) {
  return static_cast<std::size_t>(get_consistent_local_bounds(p_text).height);
}