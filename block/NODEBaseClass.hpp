#ifndef NODEBASECLASS_HPP
#define NODEBASECLASS_HPP

#include "../Globals.hpp"
#include "Dropdown.hpp"
#include "LineInput.hpp"

class NODEBaseClass {

protected:
  std::string text;

public:
  NODE_TYPE type = NODE_TYPE::PLACEHOLDER;
  std::string bind_string;
  bool pressed = false;
  sf::Vector2f _pos{0.0f, 0.0f};

  NODEBaseClass(const std::string &p_text_str,
                const std::string &p_bind_str = "")
      : text(p_text_str), bind_string(p_bind_str){};

  sf::Vector2f min_size() { return {35.0f, 45.0f}; }

  virtual sf::Vector2f rect_size() = 0;

  bool is_mouse_over(sf::Vector2f this_nodes_pos) {
    sf::Vector2f size = rect_size();
    return (mouse_position.x >= this_nodes_pos.x &&
            mouse_position.x <= this_nodes_pos.x + size.x) &&
           (mouse_position.y >= this_nodes_pos.y &&
            mouse_position.y <= this_nodes_pos.y + size.y);
  };

  virtual std::string get_text() { return text; }
  virtual void set_text(const std::string &str) { text = str; }

  virtual bool left_click_action() { return false; };

  virtual void _process_event(sf::Event event){};

  virtual void Render(sf::Vector2f pos) = 0;
};

class LabelNode : public NODEBaseClass {
public:
  LabelNode(const std::string &p_text_str, const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LABEL;
  }

  sf::Vector2f rect_size() override {
    return {std::max(text_rect_size(text).x, min_size().x) + 15, min_size().y};
  }

  void Render(sf::Vector2f pos) override { draw_text(text, pos); }
};

class ButtonNode : public NODEBaseClass {
public:
  ButtonNode(const std::string &p_text_str, const std::string &p_bind_str = "")
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

private:
  LineInput text_area;

public:
  LineInputAttachFieldNode(const std::string &p_text_str,
                           const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LINE_INPUT_ATTACH_FIELD;
    text_area.line_input_active = false;
  }

  sf::Vector2f rect_size() override { return text_area.rect_size(); }

  std::string get_text() override { return text_area.input_text; }
  void set_text(const std::string &str) override { text_area.input_text = str; }

  bool left_click_action() override {
    pressed = true;
    text_area.line_input_active = true;
    return true;
  }

  void _process_event(sf::Event event) override {
    text_area.handle_inputs(event);
  }

  void Render(sf::Vector2f pos) override {
    text_area.position = pos;
    text_area.Render();
  }
};

class DropDownNode : public NODEBaseClass {

private:
  DropDown dropdown;

public:
  DropDownNode(const std::string &p_text_str,
               const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::DROP_DOWN;
    dropdown.items.clear();
    dropdown.items = {"W", "S", "A", "D"};
  }

  sf::Vector2f rect_size() override { return dropdown.rect_size(); }

  std::string get_text() override { return dropdown.get_text(); }

  bool left_click_action() override {
    // dropdown.dropdown_clicked = true;
    return true;
  }

  void _process_event(sf::Event event) override {
    dropdown.handle_inputs(event);
  }

  void Render(sf::Vector2f pos) override {
    dropdown.position = pos;
    dropdown.Render();
  }
};

class BlockAttachNode : public NODEBaseClass {
public:
  // TODO-Handle the size of all the blocks attached to it.

  // The speciality of this node, is that it draws a 'L' shape where other
  // blocks can be snapped to it. New blocks are snapped on the top of the so
  // 'L' shape. This class itself handles the giving of the snap hint. However,
  // we don't store the pointer to the block that is attached to it, to reduce
  // dependencies. That is handled by the Block class.

  BlockAttachNode(const std::string &p_text_str,
                  const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::BLOCK_ATTACH_NODE;
  }

  sf::Vector2f rect_size() override { return {15, 2 * 45.0f}; }

  void Render(sf::Vector2f pos) override {
    // Draw the long vertical line on the left.
    sf::RectangleShape r;
    r.setPosition(pos);
    r.setSize(rect_size());
    r.setFillColor(sf::Color::Yellow);
    window.draw(r);

    // The horizontal block on the bottom.
    // Which marks the end of the block.
    sf::RectangleShape r2;
    r2.setPosition(pos + sf::Vector2f{0.0, rect_size().y});
    // A block is 45 units.
    r2.setSize({300, 45.0f});
    r2.setFillColor(sf::Color::Yellow);
    window.draw(r2);
  }
};

#endif // NODEBASECLASS_HPP