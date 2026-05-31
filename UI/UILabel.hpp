#ifndef UI_LABEL
#define UI_LABEL

#include "UIBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <string>

inline sf::Font LabelFont;

class UILabel : public UIBaseClass {
private:
  sf::Text text;
  sf::RectangleShape outline;

  void update_internal();

public:
  sf::Vector2f getPosition() override;

  sf::Vector2f get_actual_text_position();
  sf::Vector2f get_actual_text_size();

  void setPosition(sf::Vector2f) override;

  sf::Vector2f rect_size() override;

  void Render() override;

  void RenderDebug() override;

  const sf::RectangleShape &get_rectangle() { return outline; }

  sf::Text &get_label() { return text; }

  void set_text(const std::string &str);

  std::string get_text();

  void handle_inputs(sf::Event event) override {}

  UILabel(const std::string &str = "",
          const sf::Vector2f pos = sf::Vector2f(0.0F, 0.0F));

  /// @brief Returns the maximum character height for a given sf::Text.
  static std::size_t max_character_size(sf::Text &p_text);

  /// @brief Returns stable local bounds (top/height driven by full font
  ///        metrics) regardless of which characters are in the string.
  ///        Results are cached per (font*, characterSize) pair.
  static sf::FloatRect get_consistent_local_bounds(const sf::Text &p_text);
};

#endif // UI_LABEL