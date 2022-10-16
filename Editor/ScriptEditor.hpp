#ifndef SCRIPT_EDITOR
#define SCRIPT_EDITOR

#include "../block/Block.hpp"
#include "Script.hpp"

class ScriptEditor {
private:
  bool middle_click = false;

  Block *get_block_it_is_attached_to(Block *block_to_test);
  Block *get_currently_dragging_block();
  void Update();

public:
  Script *script = nullptr;

  bool is_any_block_dragging();

  void reset_dragged_block();
  void handle_inputs(sf::Event event);
  void Render();
};

#endif // SCRIPT_EDITOR
