#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "../block/Block.hpp"
#include <vector>

class Script {
public:
  int attached_to_sprite_id = -1;
  std::vector<Block> blocks;

  Script() { blocks.reserve(16); }
};

#endif // SCRIPT_HPP
