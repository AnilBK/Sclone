#ifndef NODEBASECLASS_HPP
#define NODEBASECLASS_HPP

#include "../Globals.hpp"
#include "DropDown.hpp"
#include "LineInput.hpp"
#include <initializer_list>

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

  sf::Vector2f min_size();

  bool is_mouse_over(sf::Vector2f this_nodes_pos);

  virtual std::string get_text();
  virtual void set_text(const std::string &str);

  virtual bool left_click_action();

  virtual void _process_event(sf::Event event){};

  virtual sf::Vector2f rect_size() = 0;
  virtual void Render(sf::Vector2f pos) = 0;
};

class LabelNode : public NODEBaseClass {
public:
  LabelNode(const std::string &p_text_str, const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LABEL;
  }

  sf::Vector2f rect_size() override;

  void Render(sf::Vector2f pos) override;
};

class ButtonNode : public NODEBaseClass {
public:
  ButtonNode(const std::string &p_text_str, const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::BUTTON;
  }

  sf::Vector2f rect_size() override;

  bool left_click_action() override;

  void _draw_button(const sf::Vector2f p_position, const sf::Vector2f p_size,
                    sf::Color color = sf::Color(192, 195, 198, 255));

  void Render(sf::Vector2f pos) override;
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

  sf::Vector2f rect_size() override;

  std::string get_text() override;

  void set_text(const std::string &str);

  bool left_click_action() override;

  void _process_event(sf::Event event) override;

  void Render(sf::Vector2f pos) override;
};

class DropDownNode : public NODEBaseClass {

private:
  DropDown dropdown;

public:
  DropDownNode(const std::string &p_text_str,
               const std::string &p_bind_str = "",
               std::initializer_list<std::string> options = {})
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::DROP_DOWN;
    dropdown.items.clear();
    dropdown.items = options;
  }

  sf::Vector2f rect_size() override;

  std::string get_text() override;

  bool left_click_action() override;

  void _process_event(sf::Event event) override;

  void Render(sf::Vector2f pos) override;
};

class BlockAttachNode : public NODEBaseClass {
private:
  sf::Vector2f enclosed_rect_size{15, 2 * 45.0f};

public:
  bool draw_bottom_part = true;

  // TODO-Handle the size of all the blocks attached to it.

  // The speciality of this node, is that it draws a 'L' shape where other
  // blocks can be snapped to it. New blocks are snapped on the top of the so
  // 'L' shape. This class itself handles the giving of the snap hint. However,
  // we don't store the pointer to the block that is attached to it, to reduce
  // dependencies. That is handled by the Block class.

  BlockAttachNode(const std::string &p_text_str,
                  const std::string &p_bind_str = "",
                  bool p_draw_bottom_part = true)
      : NODEBaseClass(p_text_str, p_bind_str),
        draw_bottom_part(p_draw_bottom_part) {
    type = NODE_TYPE::BLOCK_ATTACH_NODE;
  }

  // It's size changes depends on the blocks attached to it.
  void set_enclosed_rect_size(sf::Vector2f p_size);

  sf::Vector2f rect_size() override;

  // The rect size with the L-shaped outlines.
  sf::FloatRect rect_size_with_outlines();

  void Render(sf::Vector2f pos) override;
};

#endif // NODEBASECLASS_HPP