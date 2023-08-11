#ifndef SCRIPT_EDITOR
#define SCRIPT_EDITOR

#include "../block/Block.hpp"
#include "Script.hpp"

enum class SNAP_DIRECTION { TOP, BOTTOM, INSIDE, NONE };

struct SNAP_HINT_INFO {
  bool can_snap;
  SNAP_DIRECTION snap_direction;
  Block *block_to_snap_to;
  BlockAttachNode *attach_block_inside_node;
};

class ScriptEditor {
private:
  sf::View view;
  sf::FloatRect world;
  sf::RectangleShape border;

  sf::Vector2f old_mouse_pos, new_mouse_pos;

  void handle_panning(sf::Event event);

  Block *get_block_it_is_attached_to(Block *block_to_test);
  Block *get_currently_dragging_block();

  SNAP_HINT_INFO get_block_to_snap_on(Block *current_dragging_block_ref);
  void ShowSnapHints();

public:
  Script *script = nullptr;

  bool is_any_block_dragging();

  void reset_dragged_block();
  void handle_inputs(sf::Event event);
  void Render();

  void setRect(sf::FloatRect rect);

  sf::View &get_view() { return view; }

  sf::FloatRect &get_world() { return world; }
};

#endif // SCRIPT_EDITOR
