#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Globals.hpp"
#include "NODEBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>

// Final Block Output:
// VVVVVVVVVVVVVVVVVVVVVVVVVV
// Say "Text" for "2" seconds.
// The Generator API.
//   Label("Say")
//     LineInputAttachField(text_variable)
//       Label("for")
//         LineInputAttachField(time_variable)
//           Label("seconds").

// With this new API.
// Print "String" becomes
//   Label("Print")
//     LineInputAttachField(Str)

class Block {

private:
  const float spacing = 10, padding_left = 5, padding_right = 5, padding_up = 5,
              padding_down = 5;
  const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

  sf::RectangleShape block_rect;

  sf::Vector2f position;

  BLOCK_TYPES block_type;
  bool can_block_snap_inside = false;

  sf::FloatRect _previous_block_snap_rect();
  sf::FloatRect _next_block_snap_rect();

public:
  Block();

  bool dragging = false;

  Block *next_block = nullptr;
  // These are the blocks that are attached inside a block.
  // For a given block these exist only if 'BlockAttachNode' nodes are attached
  // to it.
  std::vector<Block *> attached_blocks;
  // Make this vvvv function handle top and bottom snap highlights as well.
  void show_inside_snap_hints(bool attach_block_requested,
                              Block *current_dragging_block_ref);

  std::function<std::string(Block b)> output_code_callback;
  std::vector<std::shared_ptr<NODEBaseClass>> childrens;

  void set_position(const sf::Vector2f pos);

  void set_block_type(BLOCK_TYPES p_type) {
    block_type = p_type;
    if (block_type == BLOCK_TYPES::CONTROL) {
      block_rect.setFillColor(sf::Color::Yellow);
    } else {
      block_rect.setFillColor(sf::Color::Green);
    }
  }

  template <class T> void add_node(T node_class) {
    if (node_class.type == NODE_TYPE::BLOCK_ATTACH_NODE) {
      can_block_snap_inside = true;
    }
    auto u_block = std::make_shared<T>(node_class);
    childrens.push_back(std::move(u_block));
  }

  std::optional<std::string> get_bound_value(const std::string &query);

  bool can_mouse_snap_to_top();
  bool can_mouse_snap_to_bottom();

  void attach_block_next(Block *p_next_block);

  void show_previous_block_snap_hint();
  void show_next_block_snap_hint();

  std::string get_code();
  bool is_control_block();

  // Maybe use some dirty flag ???
  void _recalculate_rect();
  bool _process_left_click_on_children(sf::Event event);
  void _process_events(sf::Event event);

  bool _any_node_already_pressed();
  void _deselect_all_nodes();

  void Render();
};

#endif
