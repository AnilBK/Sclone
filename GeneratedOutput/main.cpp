// Generated Code.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

sf::Font font;
sf::RenderWindow window;

// BUBBLE-SPEECH_BEGIN
sf::Text bubble;
void init_bubble_label() {
  bubble.setFont(font);
  bubble.setCharacterSize(32);
  bubble.setPosition(sf::Vector2f(20, 20));
  bubble.setFillColor(sf::Color::White);
}

class bubble_message {
public:
  sf::Sprite *target_sprite_ptr;
  float length; // Show the message for this much seconds.
  std::string message;
};

bool bubble_sys_update_required = true;
float bubble_message_system_timer = 0.0f;
std::vector<bubble_message> bubble_messages;

void add_bubble_message(sf::Sprite *target_sprite_ptr, float length,
                        std::string message) {
  bubble_message new_msg;
  new_msg.target_sprite_ptr = target_sprite_ptr;
  new_msg.length = length;
  new_msg.message = message;
  bubble_messages.push_back(new_msg);

  bubble_sys_update_required = true;
}

void update_bubble_message_system(float delta_time) {
  // No messages in the queue.
  // So,no update required.
  if (!bubble_sys_update_required) {
    return;
  }

  if (bubble_messages.empty()) {
    return;
  }

  auto bubble_msg = bubble_messages.front();
  auto msg_target_sprite = bubble_msg.target_sprite_ptr;
  auto msg_length = bubble_msg.length;
  auto msg = bubble_msg.message;

  if (msg_target_sprite == nullptr) {
    return;
  }

  bubble.setString(msg);
  auto buble_rect = bubble.getGlobalBounds();
  auto bubble_size_x = buble_rect.width;

  // Orient the text message so that the centre of the text is vertically above
  // the sprite's centre.
  bubble.setPosition(
      msg_target_sprite->getPosition() +
      sf::Vector2f((msg_target_sprite->getGlobalBounds().width * 0.5f) -
                       (bubble_size_x * 0.5f),
                   -msg_target_sprite->getGlobalBounds().height));
  window.draw(bubble);

  bubble_message_system_timer += delta_time;

  if (bubble_message_system_timer >= msg_length) {
    // Reset the timer.
    bubble_message_system_timer = 0.0f;
    // Remove that message from the system.
    bubble_messages.erase(bubble_messages.begin());
    // If no message remain in the system, reset everything and disable the
    // system.
    if (bubble_messages.empty()) {
      bubble_message_system_timer = 0.0f;
      bubble_sys_update_required = false;
    }
  }
}

// BUBBLE-SPEECH_END

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
  if (!font.loadFromFile("alaska.ttf")) {
    std::cout << "Error Loading Font. \n";
    exit(1);
  }

  init_bubble_label();

  int height = sf::VideoMode::getDesktopMode().height;
  int width = sf::VideoMode::getDesktopMode().width;
  window.create(sf::VideoMode(width, height), "SClone Generated Output");

  window.setVerticalSyncEnabled(true);

  sf::RectangleShape player({50, 50});
  player.setPosition(200, 200);
  player.setFillColor(sf::Color::Red);

  sf::Texture cat_texture;
  if (!cat_texture.loadFromFile("cat.png")) {
    std::cerr << "Error while loading texture" << std::endl;
    return -1;
  }
  cat_texture.setSmooth(true);

  sf::Sprite cat;
  cat.setTexture(cat_texture);
  sf::FloatRect catSize = cat.getGlobalBounds();
  cat.setOrigin(catSize.width / 2., catSize.height / 2.);
  cat.setPosition(window.getSize().x / 2., window.getSize().y / 2.);

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

    window.clear();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      add_bubble_message(&cat, 1.5, "Hello from the bubble.");
    }

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

    window.draw(player);

    window.draw(cat);

    update_bubble_message_system(deltaTime.asSeconds());

    window.display();
  }
}
