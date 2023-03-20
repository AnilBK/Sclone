#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Globals.hpp"
#include "NODEBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>

const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

class Block {

private:
  static constexpr float spacing = 10, padding_left = 5, padding_right = 5,
                         padding_up = 5, padding_down = 5;
  sf::Vector2f position;
  BLOCK_TYPES block_type;

  /// @brief 'BlockAttachNodes' can attach another block inside. This is used to
  /// flag the presence of such nodes.
  bool can_block_attach_inside = false;

  /// @brief Adjust the positions of children to fit with others, as the sizes
  /// of children can change during the course of program execution.
  void resort_children();

  /// @brief Render the rect that encloses this whole block. Used for debugging.
  void render_full_rect();

  /// @brief Render the base block. This is the thing that can be moved around
  /// with mouse.
  void render_base();

  /// @brief Render all the UI Components like text, inputs etc contained in the
  /// block.
  void render_children();

  sf::FloatRect _previous_block_snap_rect();
  sf::FloatRect _next_block_snap_rect();

  /// @brief The size of the base block. The base block is the one that can be
  /// dragged.
  /// @return The size of the base block.
  sf::Vector2f base_size();

  /// @brief Perform actions associated with some children's on left click.
  /// @return Returns true if any of the children performed 'press' action.
  bool _process_left_click_on_children(sf::Event event);

  bool any_node_already_pressed();
  void deselect_all_nodes();

  /// @brief Get 'BlockAttachNodes' which has blocks attached to them.
  /// @return Pointers of 'BlockAttachNodes'.
  std::vector<BlockAttachNode *>
  get_block_attach_nodes(bool with_nodes_attached = true);

  /// @brief This replaces macros(shortcuts) that can be used in editor, with
  /// their correct expansions.
  void _apply_editor_shortcut_macros(std::string &code);

  /// @brief We use it to get the size of the block without the attached block.
  /// That size is used to calculate the position of the block attached to it.
  sf::FloatRect block_without_attached_block_size;

public:
  Block();
  ~Block();

  bool dragging = false;

  /// @brief The base rectangle which draws the entire shape of the block.
  sf::RectangleShape block_rect;

  sf::Vector2f block_full_size{0.0f, 0.0f};

  /// @brief This is the tab name in the editor where this block can be spawned
  /// from.
  BLOCKS_TAB_NAME TabItBelongsToName = BLOCKS_TAB_NAME::TAB_CONTROL;

  std::vector<std::shared_ptr<NODEBaseClass>> childrens;

  Block *next_block = nullptr;

  /// @brief Use this block identifier to spawn new blocks of this type.
  std::string function_identifier = "block_default";

  /// @brief Callback to the code generator for the main loop.
  std::function<std::string(const Block &b)> output_code_callback;

  /// @brief Stores initialization callbacks, as some objects require
  /// initialization of some data before entering the main loop.
  std::function<std::string(const Block &b)> output_code_for_init_callback;

  template <class T> void add_node(T node_class) {
    if (node_class.type == NODE_TYPE::BLOCK_ATTACH_NODE) {
      can_block_attach_inside = true;
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
  void process_inside_snap_hints(bool attach_block_requested,
                                 Block *current_dragging_block_ref);

  /// @brief The rect that encloses this whole block.
  /// @return The enclosing rect.
  sf::FloatRect full_rect();

  void update_children_sizes();

  void handle_inputs(sf::Event event);
  void Render();

  std::string get_code();
  std::string get_code_for_init();

  std::optional<std::string> get_bound_value(const std::string &query) const;
};

namespace BLOCK_UTILITIES {
bool is_forever_block(const Block &block);
bool is_input_block(const Block &block);
bool is_program_starts_block(const Block &block);
} // namespace BLOCK_UTILITIES

#endif
