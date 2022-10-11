#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "block/Block.hpp"
#include <vector>

class Script {
public:
  int attached_to_sprite_id = -1;
  std::vector<Block> blocks;

  Script() { blocks.reserve(16); }
};

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

#endif // SCRIPT_HPP
