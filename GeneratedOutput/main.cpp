// Generated Code.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

sf::RenderWindow window;

// TODO:Add these Vector2f functions to my custom sfml build.
float distance_to(sf::Vector2f p1, sf::Vector2f p2) {
  auto x_dt = (p2.x - p1.x);
  auto y_dt = (p2.y - p1.y);

  return sqrt((x_dt * x_dt) + (y_dt * y_dt));
}

sf::Vector2f normalized(sf::Vector2f vec) {
  float l = (vec.x * vec.x) + (vec.y * vec.y);
  if (l != 0.0f) {
    float mag = sqrt(l);
    vec.x /= mag;
    vec.y /= mag;
  }
  return vec;
}

bool is_mouse_over(sf::Sprite *sprite) {
  sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
  return sprite->getGlobalBounds().contains(sf::Vector2f(mouse_position));
}

int main() {

  sf::Font text_font;
  if (!text_font.loadFromFile("alaska.ttf")) {
    std::cout << "Error Loading Font. \n";
    exit(1);
  }

  unsigned int height = sf::VideoMode::getDesktopMode().height;
  unsigned int width = sf::VideoMode::getDesktopMode().width;
  window.create(sf::VideoMode(width, height), "SClone Generated Output");

  window.setVerticalSyncEnabled(true);

  sf::Texture Cat_texture;
  if (!Cat_texture.loadFromFile("cat.png")) {
    std::cerr << "Error while loading texture" << std::endl;
    return -1;
  }
  Cat_texture.setSmooth(true);

  sf::Sprite Cat;
  Cat.setTexture(Cat_texture);
  sf::FloatRect CatSize = Cat.getGlobalBounds();
  Cat.setOrigin(CatSize.width / 2.0f, CatSize.height / 2.0f);
  Cat.setPosition(200, 384);

  int _Cat_score = 0;
  int _Cat_health = 0;
  sf::Text Cat__text;
  Cat__text.setPosition({200, 184});
  Cat__text.setFont(text_font);
  Cat__text.setCharacterSize(24);
  Cat__text.setFillColor(sf::Color::Red);

  ///////////////////////////////////////
  ///////////////////////////////////////

  sf::Clock frameClock;
  const auto window_clear_color = sf::Color(153, 195, 180);

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }

      if (e.type == sf::Event::MouseButtonReleased &&
          e.mouseButton.button == sf::Mouse::Left && is_mouse_over(&Cat)) {
        _Cat_score = 1;
      }
    }

    window.clear(window_clear_color);
    auto deltaTime = frameClock.restart();

    Cat__text.setString("Score" + std::to_string(_Cat_score) + " Health" +
                        std::to_string(_Cat_health) + "");
    window.draw(Cat__text);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      _Cat_health = 1;
    }

    window.draw(Cat);

    window.display();
  }
}
