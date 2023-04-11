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
  sf::Vector2f interpolated_pos, unit_vec;

  float animation_length = 0.0f, time_elapsed = 0.0f;

  sf::Sprite *target_sprite_ptr;
  sf::Vector2f target_pos;

public:
  bool offset_target_position = false;

  bool is_valid() const { return time_elapsed <= animation_length; }

  void initialize() {
    const sf::Vector2f starting_pos = target_sprite_ptr->getPosition();

    if (offset_target_position) {
      target_pos += starting_pos;
    }

    interpolated_pos = starting_pos;
    const float movement_speed =
        distance_to(starting_pos, target_pos) / animation_length;
    unit_vec = normalized(target_pos - starting_pos) * movement_speed;
  }

  void update(float delta) {
    time_elapsed += delta;
    interpolated_pos += (unit_vec * delta);

    if (target_sprite_ptr) {
      target_sprite_ptr->setPosition(interpolated_pos);
    }
  }

  move_to_point_data(sf::Sprite *p_target_sprite_ptr, const float p_anim_length,
                     const sf::Vector2f p_target_pos)
      : animation_length(p_anim_length), target_sprite_ptr(p_target_sprite_ptr),
        target_pos(p_target_pos){};
};

std::vector<move_to_point_data> move_to_point_datas;

void add_move_to_point_operation(sf::Sprite *p_target_sprite_ptr,
                                 sf::Vector2f p_target, float p_length) {
  move_to_point_data mv_data(p_target_sprite_ptr, p_length, p_target);
  move_to_point_datas.push_back(mv_data);
}

void add_change_offset_operation(sf::Sprite *p_target_sprite_ptr,
                                 sf::Vector2f p_offset, float p_length) {
  // Save the offset in the target position.
  move_to_point_data mv_data(p_target_sprite_ptr, p_length, p_offset);
  mv_data.offset_target_position = true;
  // Use the 'offset_target_position' flag later in initialization to set the
  // exact position.
  move_to_point_datas.push_back(mv_data);
}

void update_move_to_point_system(float delta_time) {
  if (move_to_point_datas.empty()) {
    return;
  }

  auto &curr_anim_ref = move_to_point_datas.at(0);

  static bool initialized = false;
  if (!initialized) {
    curr_anim_ref.initialize();
    initialized = true;
  }

  curr_anim_ref.update(delta_time);

  if (!curr_anim_ref.is_valid()) {
    move_to_point_datas.erase(move_to_point_datas.begin());
    initialized = false;
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
        add_change_offset_operation(&Cat, sf::Vector2f(200, 0), 1);
        add_change_offset_operation(&Cat, sf::Vector2f(0, 100), 1);
      }
    }

    window.clear(window_clear_color);
    auto deltaTime = frameClock.restart();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      add_move_to_point_operation(&Cat, sf::Vector2f(0, 0), 2);
    }

    window.draw(Cat);

    update_move_to_point_system(deltaTime.asSeconds());

    window.display();
  }
}
