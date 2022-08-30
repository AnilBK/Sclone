#ifndef NODEBASECLASS_HPP
#define NODEBASECLASS_HPP

#include "../Globals.hpp"

class NODEBaseClass {
public:
  std::string text;
  NODE_TYPE type;
  std::string bind_string;
  bool pressed = false;

  NODEBaseClass(std::string p_text_str, std::string p_bind_str = "") {
    text = p_text_str;
    bind_string = p_bind_str;
  };

  sf::Vector2f min_size() { return {35.0f, 45.0f}; }

  virtual sf::Vector2f rect_size() = 0;

  bool is_mouse_over(sf::Vector2f this_nodes_pos) {
    sf::Vector2f size = rect_size();
    return (mouse_position.x >= this_nodes_pos.x &&
            mouse_position.x <= this_nodes_pos.x + size.x) &&
           (mouse_position.y >= this_nodes_pos.y &&
            mouse_position.y <= this_nodes_pos.y + size.y);
  };

  virtual bool left_click_action() { return false; };

  virtual void Render(sf::Vector2f pos) = 0;
};

class LabelNode : public NODEBaseClass {
public:
  LabelNode(std::string p_text_str, std::string p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LABEL;
  }

  sf::Vector2f rect_size() override {
    return {text_rect_size(text).x, min_size().y};
  }

  void Render(sf::Vector2f pos) override { draw_text(text, pos); }
};

class ButtonNode : public NODEBaseClass {
public:
  ButtonNode(std::string p_text_str, std::string p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::BUTTON;
  }

  sf::Vector2f rect_size() override {
    sf::Vector2f text_box_size{text_rect_size(text)};
    sf::Vector2f btn_size{std::max(text_box_size.x, min_size().x) + 5,
                          std::max(text_box_size.y, min_size().y)};
    return btn_size;
  }

  bool left_click_action() override {
    pressed = true;
    return true;
  }

  void _draw_button(const sf::Vector2f p_position, const sf::Vector2f p_size,
                    sf::Color color = sf::Color(192, 195, 198, 255)) {
    sf::RectangleShape r;
    r.setPosition(p_position);
    r.setSize(p_size);
    r.setFillColor(color);
    window.draw(r);
  }

  void Render(sf::Vector2f pos) override {
    sf::Vector2f btn_size = rect_size();

    _draw_button(pos, btn_size);
    if (pressed) {
      sf::Vector2f pressed_highlight_pos =
          pos + sf::Vector2f{0, (float)(0.85 * btn_size.y)};
      sf::Vector2f pressed_highlight_size = {btn_size.x,
                                             (float)(0.15 * btn_size.y)};
      _draw_button(pressed_highlight_pos, pressed_highlight_size,
                   {41, 44, 247, 255});
    }

    draw_text(text, pos);
  }
};

class LineInputAttachFieldNode : public NODEBaseClass {
public:
  LineInputAttachFieldNode(std::string p_text_str, std::string p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LINE_INPUT_ATTACH_FIELD;
  }

  sf::Vector2f rect_size() override {
    auto size = min_size();
    if (text != "") {
      size.x = std::max(text_rect_size(text).x, min_size().x);
    }
    return size;
  }

  bool left_click_action() override {
    pressed = true;
    return true;
  }

  void _draw_line_input_attach_field(const sf::Vector2f p_position) {
    sf::RectangleShape r;
    r.setPosition(p_position);
    r.setSize(rect_size());
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

  void Render(sf::Vector2f pos) override {
    _draw_line_input_attach_field(pos);
    if (text != "") { // Usually empty for line input attach field so.
      draw_text(text, pos);
    }
  }
};

#endif // NODEBASECLASS_HPP