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

const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

class Block {

private:
  static constexpr float spacing = 10, padding_left = 5, padding_right = 5,
                         padding_up = 5, padding_down = 5;

  sf::Vector2f position;

  BLOCK_TYPES block_type;
  bool can_block_snap_inside = false;

  sf::FloatRect _previous_block_snap_rect();
  sf::FloatRect _next_block_snap_rect();

  /// @brief Get 'BlockAttachNodes' which has blocks attached to them.
  /// @return Pointers of 'BlockAttachNodes'.
  std::vector<BlockAttachNode *>
  get_block_attach_nodes(bool with_nodes_attached = true);

  // Recalculates children postions.
  void resort_children();
  void render_children();
  void RenderFullRect();

public:
  Block();
  ~Block();

  bool dragging = false;

  sf::RectangleShape block_rect;

  sf::FloatRect full_rect();

  sf::Vector2f block_full_size{0.0f, 0.0f};
  // This is the tab name in the editor where this block can be spawned from..
  // std::string TabItBelongsToName = "Control";
  // String would be flexible but use enums for speed now.
  BLOCKS_TAB_NAME TabItBelongsToName = BLOCKS_TAB_NAME::TAB_CONTROL;

  std::vector<std::shared_ptr<NODEBaseClass>> childrens;

  Block *next_block = nullptr;
  // These are the blocks that are attached inside a block.
  // For a given block these exist only if 'BlockAttachNode' nodes are attached
  // to it.

  // We use this block identifier to spawn new blocks.
  std::string function_identifier = "block_default";
  std::function<std::string(const Block &b)> output_code_callback;

  template <class T> void add_node(T node_class) {
    if (node_class.type == NODE_TYPE::BLOCK_ATTACH_NODE) {
      can_block_snap_inside = true;
      block_rect.setFillColor(sf::Color::Yellow);
    }
    auto u_block = std::make_shared<T>(node_class);
    childrens.push_back(std::move(u_block));
  }

  bool is_mouse_over();
  bool is_control_block();

  void set_block_type(BLOCK_TYPES p_type);
  void attach_block_next(Block *p_next_block);

  void set_position(const sf::Vector2f p_pos);

  bool can_mouse_snap_to_top();
  bool can_mouse_snap_to_bottom();

  void show_previous_block_snap_hint();
  void show_next_block_snap_hint();
  // Make this vvvv function handle top and bottom snap highlights as well.
  void process_inside_snap_hints(bool attach_block_requested,
                                 Block *current_dragging_block_ref);

  // Maybe use some dirty flag ???
  void _recalculate_rect();
  bool _process_left_click_on_children(sf::Event event);
  void _process_events(sf::Event event);

  bool _any_node_already_pressed();
  void _deselect_all_nodes();

  void update_children_sizes();

  void Render();
  void RenderRectsBackground();
  void RenderComponents();

  std::string get_code();
  std::optional<std::string> get_bound_value(const std::string &query) const;
};

#endif
