// Generated Code.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

sf::Vector2f normalized(sf::Vector2f vec) {
  float l = (vec.x * vec.x) + (vec.y * vec.y);
  if (l != 0.0f) {
    float mag = sqrt(l);
    vec.x /= mag;
    vec.y /= mag;
  }
  return vec;
};

int main() {
  sf::RenderWindow window;
  int height = sf::VideoMode::getDesktopMode().height;
  int width = sf::VideoMode::getDesktopMode().width;
  window.create(sf::VideoMode(width, height), "SClone Generated Output");

  window.setVerticalSyncEnabled(true);

  sf::RectangleShape player({50, 50});
  player.setPosition(200, 200);
  player.setFillColor(sf::Color::Red);

  //###INIT_CODES###

  ///////////////////////////////////////
  ///////////////////////////////////////

  sf::Vector2f velocity;

  sf::Clock frameClock;

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }
    }

    //###MAINLOOP_CODE###

    velocity = {0.0f, 0.0f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      velocity.x += 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      velocity.x -= 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      velocity.y += 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      velocity.y -= 1.0f;
    }

    int speed = 200;
    velocity = normalized(velocity);
    velocity.x *= speed;
    velocity.y *= speed;

    auto deltaTime = frameClock.restart();
    player.move(velocity * deltaTime.asSeconds());

    window.clear();
    window.draw(player);

    //###RENDER_CODE###

    window.display();
  }
}
