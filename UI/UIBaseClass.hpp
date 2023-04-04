#ifndef UI_BASE_CLASS
#define UI_BASE_CLASS

#include "../Core/Object.hpp"
#include "../Globals.hpp"
#include <SFML/Graphics.hpp>

class UIBaseClass : public Object {

public:
  virtual sf::Vector2f getPosition() = 0;
  virtual void setPosition(sf::Vector2f) = 0;
  virtual sf::Vector2f rect_size() = 0;

  virtual void RenderDebug(){};
  virtual void Render() = 0;

  virtual void handle_inputs(sf::Event event) = 0;

  bool is_mouse_over() {
    sf::Vector2f size = rect_size();
    sf::Vector2f position = getPosition();

    return isMouseOverRect(sf::FloatRect(position, size));
  }
};

#endif // UI_BASE_CLASS
