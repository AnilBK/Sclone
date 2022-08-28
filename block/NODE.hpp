#ifndef NODE_HPP
#define NODE_HPP

#include "../Globals.hpp"

struct NODE {
  std::string text;
  NODE_TYPE type;
  // A Node like a button has a callback on mouse click.
  // std::function<void()> callback;
  bool pressed = false;

  sf::Vector2f line_input_field_rect_size() { return {75, 35}; }

  float get_rect_size_x() {
    if (type == NODE_TYPE::LABEL) {
      return text_rect_size(text).x;
    } else if (type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
      if (text != "") {
        return std::max(text_rect_size(text).x, line_input_field_rect_size().x);
      }
      return line_input_field_rect_size().x;
    } else if (type == NODE_TYPE::BUTTON) {
      sf::Vector2f size{30, 40};
      sf::Vector2f text_box_size{text_rect_size(text)};
      sf::Vector2f btn_size{std::max(text_box_size.x, size.x) + 5,
                            std::max(text_box_size.y, size.y)};
      return btn_size.x;
    }

    return 35.0f;
  }

  //  This and above function should be merged sensibly.
  sf::Rect<float> get_rect(sf::Vector2f pos_at) {
    // const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};
    sf::Vector2f size{get_rect_size_x(), 45.0f};
    return {pos_at, size};
  }

  bool is_mouse_over(sf::Vector2f this_nodes_pos) {
    sf::Vector2f size{get_rect_size_x(), 45.0f};
    return (mouse_position.x >= this_nodes_pos.x &&
            mouse_position.x <= this_nodes_pos.x + size.x) &&
           (mouse_position.y >= this_nodes_pos.y &&
            mouse_position.y <= this_nodes_pos.y + size.y);
  }

  bool is_mouse_picker_button() {
    return type == NODE_TYPE::BUTTON && text == "Pick^";
  }

  bool left_click_action() {
    if ((type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) ||
        (type == NODE_TYPE::BUTTON)) {
      pressed = true;
      return true;
    }

    return false;
  }

  void draw_button(const sf::Vector2f p_position, const sf::Vector2f p_size,
                   sf::Color color = sf::Color(192, 195, 198, 255)) {
    sf::RectangleShape r;
    r.setPosition(p_position);
    r.setSize(p_size);
    r.setFillColor(color);
    window.draw(r);
  }

  void draw_line_input_attach_field(const sf::Vector2f p_position) {
    sf::RectangleShape r;
    r.setPosition(p_position);
    r.setSize(line_input_field_rect_size());
    r.setFillColor(sf::Color::White);
    // A shadow when a button is highlighted.
    if (pressed) {
      r.setOutlineThickness(2.5);
      r.setOutlineColor(sf::Color(31, 142, 255, 255));
    } else {
      r.setOutlineThickness(2.0);
      r.setOutlineColor(sf::Color(91, 91, 91, 255));
    }
    window.draw(r);
  }

  void Render(sf::Vector2f pos) {
    if (type == NODE_TYPE::LABEL) {
      draw_text(text, pos);
    } else if (type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
      draw_line_input_attach_field(pos);
      if (text != "") { // Usually empty for line input attach field so.
        draw_text(text, pos);
      }
    } else if (type == NODE_TYPE::BUTTON) {
      sf::Vector2f size{30, 40};
      sf::Vector2f text_box_size{text_rect_size(text)};
      sf::Vector2f btn_size{std::max(text_box_size.x, size.x) + 5,
                            std::max(text_box_size.y, size.y)};
      draw_button(pos, btn_size);
      if (pressed) {
        sf::Vector2f pressed_highlight_pos =
            pos + sf::Vector2f{0, (float)(0.85 * btn_size.y)};
        sf::Vector2f pressed_highlight_size = {btn_size.x,
                                               (float)(0.15 * btn_size.y)};
        draw_button(pressed_highlight_pos, pressed_highlight_size,
                    {41, 44, 247, 255});
      }

      draw_text(text, pos);
    }
  }
};

#endif
