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

  void set_text(const std::string &str);

  std::string get_text();

  void handle_inputs(sf::Event event) override {}

  UILabel(const std::string &str, const sf::Vector2f pos);
};
#endif
