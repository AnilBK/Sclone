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

int main() {

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
  Cat.setPosition(471, 420);
  Cat.setScale(4.882460, 3.446712);

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
    }

    window.clear(window_clear_color);

    auto deltaTime = frameClock.restart();
    auto delta = deltaTime.asSeconds();

    window.draw(Cat);

    window.display();
  }
}
