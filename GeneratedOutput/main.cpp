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

class move_to_point_data {
private:
  sf::Vector2f target;
  sf::Vector2f interpolated_pos;
  sf::Vector2f unit_vec;
  float length = 0.0f;

  float dt = 1.0f;
  float time_elapsed = 0.0f;

  sf::Sprite *target_sprite_ptr;
  bool initialized = false;

public:
  sf::Vector2f offset;
  bool init_late = false;

  bool is_valid() const { return time_elapsed <= length; }

  void update(float delta) {
    if (!initialized) {
      sf::Vector2f current = target_sprite_ptr->getPosition();

      if (init_late) {
        target = current + offset;
      }

      interpolated_pos = current;
      unit_vec = normalized(target - current);
      dt = distance_to(current, target) / length;
      initialized = true;
    }

    time_elapsed += delta;
    interpolated_pos += (unit_vec * dt * delta);

    if (target_sprite_ptr == nullptr) {
      std::cout << "nullptr\n";
      return;
    }
    target_sprite_ptr->setPosition(interpolated_pos);
  }

  move_to_point_data(sf::Sprite *p_target_sprite_ptr, sf::Vector2f p_target,
                     float p_length)
      : target(p_target), length(p_length) {
    target_sprite_ptr = p_target_sprite_ptr;
  }

  move_to_point_data(sf::Sprite *p_target_sprite_ptr, float p_length)
      : length(p_length) {
    target_sprite_ptr = p_target_sprite_ptr;
  }
};

std::vector<move_to_point_data> move_to_point_datas;

void add_move_to_point_operation(sf::Sprite *p_target_sprite_ptr,
                                 sf::Vector2f p_target, float p_length) {
  move_to_point_data mv_data(p_target_sprite_ptr, p_target, p_length);
  move_to_point_datas.push_back(mv_data);
}

void add_change_offset_operation(sf::Sprite *p_target_sprite_ptr,
                                 sf::Vector2f p_offset, float p_length) {
  move_to_point_data mv_data(p_target_sprite_ptr, p_length);
  mv_data.init_late = true;
  mv_data.offset = p_offset;
  move_to_point_datas.push_back(mv_data);
}

void update_move_to_point_system(float delta_time) {
  if (move_to_point_datas.empty()) {
    return;
  }

  move_to_point_datas.at(0).update(delta_time);
  if (!move_to_point_datas.at(0).is_valid()) {
    move_to_point_datas.erase(move_to_point_datas.begin());
  }
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
  Cat.setPosition(200, 384);

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
        add_change_offset_operation(&Cat, sf::Vector2f(0, 100), 2);
        add_change_offset_operation(&Cat, sf::Vector2f(100, 0), 2);
      }
    }

    window.clear(window_clear_color);
    auto deltaTime = frameClock.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      add_change_offset_operation(&Cat, sf::Vector2f(100, 0), 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      add_change_offset_operation(&Cat, sf::Vector2f(-100, 0), 1);
    }

    window.draw(Cat);

    update_move_to_point_system(deltaTime.asSeconds());

    window.display();
  }
}
