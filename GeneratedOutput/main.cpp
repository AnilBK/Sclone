// Generated Code.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

sf::Font font;
sf::RenderWindow window;

// BUBBLE-SPEECH_BEGIN
sf::Text bubble_text;
void init_bubble_label() {
  bubble_text.setFont(font);
  bubble_text.setCharacterSize(26);
  bubble_text.setFillColor(sf::Color::Black);
}

void draw_bubble_message(const std::string &message,
                         const sf::Vector2f bubble_pos) {
  // .------------.
  // .            .
  // .----- ------.
  //       v
  //       ! Bubble Position

  bubble_text.setString(message);
  bubble_text.setFillColor(sf::Color::Black);
  const auto bubble_text_size = bubble_text.getGlobalBounds().getSize();

  const sf::Vector2f padding{10.0f, 10.0f};

  //////////////////////////////////////////
  // The background of the speech bubble.//
  ////////////////////////////////////////
  sf::Vector2f bg_box_pos = bubble_pos;
  // For the callout triangle.
  bg_box_pos -= sf::Vector2f(0.0f, 10.0f);
  // To make the text same size left and right of the callout triangle.
  bg_box_pos -= sf::Vector2f(bubble_text_size.x * 0.5f, bubble_text_size.y);
  // To reach top left of the rect from the bottom, we pass throught vertical
  // padding twice and horizontal padding once.
  bg_box_pos -= sf::Vector2f(padding.x, padding.y * 2.0f);

  sf::Vector2f bg_box_size = bubble_text_size;
  // Add left/right and top/down padding to the container.
  bg_box_size += sf::Vector2f(padding.x * 2.0f, padding.y * 2.0f);

  sf::RectangleShape callout_rect;
  callout_rect.setPosition(bg_box_pos);
  callout_rect.setSize(bg_box_size);
  callout_rect.setFillColor(sf::Color::Cyan);
  window.draw(callout_rect);

  //////////////////////////////////////////
  //        The Actual Message           //
  ////////////////////////////////////////
  // Center the text to the rectangle.
  bubble_text.setOrigin(bubble_text.getGlobalBounds().getSize() / 2.0f +
                        bubble_text.getLocalBounds().getPosition());
  bubble_text.setPosition(callout_rect.getPosition() +
                          (callout_rect.getSize() / 2.0f));
  window.draw(bubble_text);

  //////////////////////////////////////////
  //  Speech Callout Triangle tail.      //
  ////////////////////////////////////////
  // The point on the bg rect where the triangle starts.

  // The formation of v of the above figure, which is called callout triangle.
  // tl......tr
  //    \   \ 
  //     \  \
  //       \\
  //         . b

  // The top left, bottom and top right points that specify the triangle.
  const sf::Vector2f tri_start_point = bubble_pos - sf::Vector2f(8.0f, 10.0f);
  const sf::Vector2f tl = tri_start_point;
  const sf::Vector2f b = bubble_pos;
  const sf::Vector2f tr = tri_start_point + sf::Vector2f(10.0f, 0.0f);

  sf::ConvexShape callout_triangle;
  callout_triangle.setPointCount(3);
  callout_triangle.setPoint(0, tl);
  callout_triangle.setPoint(1, b);
  callout_triangle.setPoint(2, tr);
  callout_triangle.setFillColor(sf::Color::Cyan);

  window.draw(callout_triangle);
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
                        const std::string &message) {
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

  auto sprite_bubble_pos = msg_target_sprite->getGlobalBounds().getPosition();
  sprite_bubble_pos +=
      sf::Vector2f(msg_target_sprite->getGlobalBounds().width * 0.5f, -10.0f);

  draw_bubble_message(msg, sprite_bubble_pos);

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

// For move to Vector2f block.
// TODO: Generalize to Create AnimationPlayers.
class move_p2p_data {
private:
  sf::Vector2f current;
  sf::Vector2f target;
  sf::Vector2f interpolated_pos;
  sf::Vector2f unit_vec;
  float length = 0.0f;

  float dt = 1.0f;
  float time_elapsed = 0.0f;

  sf::Sprite *target_sprite_ptr;

public:
  bool is_valid() const { return time_elapsed <= length; }

  void update(float delta) {
    time_elapsed += delta;
    interpolated_pos += (unit_vec * dt * delta);
    target_sprite_ptr->setPosition(interpolated_pos);
  }

  move_p2p_data(sf::Sprite *p_target_sprite_ptr, sf::Vector2f p_current,
                sf::Vector2f p_target, float p_length)
      : current(p_current), target(p_target), interpolated_pos(current),
        length(p_length) {
    target_sprite_ptr = p_target_sprite_ptr;
    unit_vec = normalized(target - current);
    dt = distance_to(current, target) / length;
  }
};

std::vector<move_p2p_data> move_p2p_datas;

void add_move_p2p_operation(sf::Sprite *p_target_sprite_ptr,
                            sf::Vector2f p_current, sf::Vector2f p_target,
                            float p_length) {
  move_p2p_data mv_data(p_target_sprite_ptr, p_current, p_target, p_length);
  move_p2p_datas.push_back(mv_data);
}

void update_move_p2p_system(float delta_time) {
  if (move_p2p_datas.empty()) {
    return;
  }

  for (auto &mv_data : move_p2p_datas) {
    mv_data.update(delta_time);
  }

  auto remove_expired = [](const move_p2p_data &mv_data) {
    return !mv_data.is_valid();
  };

  move_p2p_datas.erase(std::remove_if(move_p2p_datas.begin(),
                                      move_p2p_datas.end(), remove_expired),
                       move_p2p_datas.end());
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
  bool is_valid() const { return time_elapsed <= length; }

  void update(float delta) {
    if (!initialized) {
      sf::Vector2f current = target_sprite_ptr->getPosition();
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
};

std::vector<move_to_point_data> move_to_point_datas;

void add_move_to_point_operation(sf::Sprite *p_target_sprite_ptr,
                                 sf::Vector2f p_target, float p_length) {
  move_to_point_data mv_data(p_target_sprite_ptr, p_target, p_length);
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

  sf::Texture star_fish_texture;
  if (!star_fish_texture.loadFromFile("fish.png")) {
    std::cerr << "Error while loading texture" << std::endl;
    return -1;
  }
  star_fish_texture.setSmooth(true);

  sf::Sprite star_fish;
  star_fish.setTexture(star_fish_texture);
  sf::FloatRect star_fishSize = star_fish.getGlobalBounds();
  star_fish.setOrigin(star_fishSize.width / 2.0f, star_fishSize.height / 2.0f);
  star_fish.setPosition(115, 553);

  sf::Texture pussy_texture;
  if (!pussy_texture.loadFromFile("cat.png")) {
    std::cerr << "Error while loading texture" << std::endl;
    return -1;
  }
  pussy_texture.setSmooth(true);

  sf::Sprite pussy;
  pussy.setTexture(pussy_texture);
  sf::FloatRect pussySize = pussy.getGlobalBounds();
  pussy.setOrigin(pussySize.width / 2.0f, pussySize.height / 2.0f);
  pussy.setPosition(598, 257);

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

    window.draw(pussy);

    update_bubble_message_system(deltaTime.asSeconds());
    update_move_p2p_system(deltaTime.asSeconds());
    update_move_to_point_system(deltaTime.asSeconds());

    window.display();
  }
}
