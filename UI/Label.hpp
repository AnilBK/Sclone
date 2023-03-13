#ifndef LABEL_HPP
#define LABEL_HPP

#include "UIBaseClass.hpp"

class Label : public UIBaseClass {
private:
  sf::Text label;

public:
  Label(const std::string &p_label = "") {
    label.setFont(font);
    label.setCharacterSize(32);
    label.setPosition(sf::Vector2f(65, 200));
    label.setFillColor(sf::Color::Black);
    setText(p_label);
  }

  sf::Vector2f getRectPosition();

  void setText(std::string txt);
  std::string getText();

  sf::Text &get_label() { return label; }

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void RenderDebug() override;
  void Render() override;

  void handle_inputs(sf::Event event) override {}
};

#endif