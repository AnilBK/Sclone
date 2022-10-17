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
private:
  float timer = 0.0f;

public:
  sf::Sprite *target_sprite_ptr;

  // A target has 'n' number of messages.
  // length_msg_pair consists of 'length of the message' and the 'actual
  // message'.
  std::vector<std::pair<float, std::string>> length_msg_pair;

  bool can_be_removed() const { return length_msg_pair.empty(); }
  void Update(float delta_time);
};

void bubble_message::Update(float delta_time) {
  if (target_sprite_ptr == nullptr) {
    return;
  }

  auto [msg_length, message] = length_msg_pair.at(0);

  const auto bounds = target_sprite_ptr->getGlobalBounds();
  auto sprite_bubble_pos = bounds.getPosition();
  sprite_bubble_pos += sf::Vector2f(bounds.width * 0.5f, -10.0f);

  draw_bubble_message(message, sprite_bubble_pos);

  timer += delta_time;
  if (timer >= msg_length) {
    timer = 0.0f;
    // Remove the first message for a given sprite as it was completed.
    length_msg_pair.erase(length_msg_pair.begin());
  }
}

std::vector<bubble_message> bubble_messages;

void add_bubble_message(sf::Sprite *target_sprite_ptr, float length,
                        const std::string &message) {
  std::pair<float, std::string> new_msg_pair{length, message};

  // If the 'target sprite' already has a message in the system, then
  // just append the new message.
  for (auto &msg : bubble_messages) {
    if (msg.target_sprite_ptr == target_sprite_ptr) {
      msg.length_msg_pair.push_back(new_msg_pair);
      return;
    }
  }

  // The 'target sprite' isn't on the system.
  // So, create an entirely new message.
  bubble_message new_msg;
  new_msg.target_sprite_ptr = target_sprite_ptr;
  new_msg.length_msg_pair.push_back(new_msg_pair);
  bubble_messages.push_back(new_msg);
}

void update_bubble_message_system(float delta_time) {
  if (bubble_messages.empty()) {
    return;
  }

  for (auto &bubble_msg : bubble_messages) {
    bubble_msg.Update(delta_time);
  }

  // Remove Completed Messages.
  bubble_messages.erase(std::remove_if(bubble_messages.begin(),
                                       bubble_messages.end(),
                                       [](const bubble_message &msg) {
                                         return msg.can_be_removed();
                                       }),
                        bubble_messages.end());
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

bool are_sprites_colliding(const sf::Sprite &a, const sf::Sprite &b) {
  auto a_bounds = a.getGlobalBounds();
  auto b_bounds = b.getGlobalBounds();

  return a_bounds.intersects(b_bounds);
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

template <class T>
void add_character_movement(T &player, sf::Time deltaTime, int speed) {
  sf::Vector2f velocity{0.0f, 0.0f};

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

  velocity = normalized(velocity);
  velocity.x *= speed;
  velocity.y *= speed;

  player.move(velocity * deltaTime.asSeconds());
}

int main() {
  if (!font.loadFromFile("alaska.ttf")) {
    std::cout << "Error Loading Font. \n";
    exit(1);
  }

  init_bubble_label();

  unsigned int height = sf::VideoMode::getDesktopMode().height;
  unsigned int width = sf::VideoMode::getDesktopMode().width;
  window.create(sf::VideoMode(width, height), "SClone Generated Output");

  window.setVerticalSyncEnabled(true);

  //###INIT_CODES###

  ///////////////////////////////////////
  ///////////////////////////////////////

  sf::Clock frameClock;

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }

      //###INPUT_CODE###
    }

    window.clear();
    auto deltaTime = frameClock.restart();

    //###MAINLOOP_CODE###

    //###RENDER_CODE###

    update_bubble_message_system(deltaTime.asSeconds());
    update_move_p2p_system(deltaTime.asSeconds());
    update_move_to_point_system(deltaTime.asSeconds());

    window.display();
  }
}