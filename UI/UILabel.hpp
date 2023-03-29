#ifndef UI_LABEL
#define UI_LABEL

#include "UIBaseClass.hpp"
#include <SFML/Graphics.hpp>

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

  /// @brief Returns the maximum character size for a given font.
  /// @param p_text Reference to the text object, whose character size is to be
  /// calculated.
  /// @return The maximum size.
  static std::size_t max_character_size(sf::Text &p_text);
};
#endif
