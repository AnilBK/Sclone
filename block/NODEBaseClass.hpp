#ifndef NODEBASECLASS_HPP
#define NODEBASECLASS_HPP

#include "../Globals.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UIDropDown.hpp"
#include "../UI/UILineInput.hpp"
#include <initializer_list>

class Block;

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

  bool is_mouse_over();

  virtual void deselect_node();

  virtual std::string get_text();
  virtual void set_text(const std::string &str);

  virtual bool left_click_action();

  virtual void handle_inputs(sf::Event event){};

  virtual sf::Vector2f rect_size() = 0;

  virtual void Render() = 0;

  // virtual void RenderOverride(sf::Vector2f pos) = 0;
};

class LabelNode : public NODEBaseClass {
public:
  LabelNode(const std::string &p_text_str, const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::LABEL;
  }

  sf::Vector2f rect_size() override;

  void Render() override;
};

class ButtonNode : public NODEBaseClass {
protected:
  UIButton btn;

public:
  ButtonNode(const std::string &p_text_str, const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str), btn(p_text_str) {
    type = NODE_TYPE::BUTTON;
    btn.is_flat = false;
    btn.button_fill_color = sf::Color(192, 195, 198, 255);
  }

  sf::Vector2f rect_size() override;

  bool left_click_action() override;

  void handle_inputs(sf::Event event) override;

  void Render() override;
};

class PickWithMouseNode : public ButtonNode {
public:
  int childs_index_for_x, childs_index_for_y;

  PickWithMouseNode(int p_index_x = -1, int p_index_y = -1)
      : ButtonNode("Pick^", ""), childs_index_for_x(p_index_x),
        childs_index_for_y(p_index_y) {}

  bool is_pressed() { return btn.is_clicked(); }

  void deselect_node() override;
};

class LineInputAttachFieldNode : public NODEBaseClass {

private:
  UILineInput text_area;

public:
  LineInputAttachFieldNode(const std::string &p_text_str,
                           const std::string &p_bind_str = "")
      : NODEBaseClass(p_text_str, p_bind_str), text_area("") {
    type = NODE_TYPE::LINE_INPUT_ATTACH_FIELD;
    text_area.line_input_active = false;
    text_area.is_flat = false;
    // Make it little bit wider than it is as used for other editor UI's.
    text_area.min_size.x = 75.0F;
  }

  sf::Vector2f rect_size() override;

  std::string get_text() override;

  void set_text(const std::string &str);

  bool left_click_action() override;

  void handle_inputs(sf::Event event) override;

  void Render() override;
};

class DropDownNode : public NODEBaseClass {

private:
  UIDropDown dropdown;

public:
  DropDownNode(const std::string &p_text_str,
               const std::string &p_bind_str = "",
               std::initializer_list<std::string> options = {})
      : NODEBaseClass(p_text_str, p_bind_str) {
    type = NODE_TYPE::DROP_DOWN;
    dropdown.clear_items();
    dropdown.add_items(options);
  }

  sf::Vector2f rect_size() override;

  std::string get_text() override;

  bool left_click_action() override;

  void handle_inputs(sf::Event event) override;

  void Render() override;
};

class BlockAttachNode : public NODEBaseClass {
private:
  sf::Vector2f enclosed_rect_size{15, 2 * 45.0f};

  sf::FloatRect _attachable_block_snap_hint_rect();

public:
  Block *attached_block;
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
      : NODEBaseClass(p_text_str, p_bind_str), attached_block(nullptr),
        draw_bottom_part(p_draw_bottom_part) {
    type = NODE_TYPE::BLOCK_ATTACH_NODE;
  }

  void _update_internal_sizes();

  // It's size changes depends on the blocks attached to it.
  void set_enclosed_rect_size(sf::Vector2f p_size);

  sf::Vector2f rect_size() override;

  void _show_snap_for_attachable_block();

  bool can_snap_block_inside();

  // The rect size with the L-shaped outlines.
  sf::FloatRect rect_size_with_outlines();

  void Render() override;
};

#endif // NODEBASECLASS_HPP