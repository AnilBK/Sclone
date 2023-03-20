#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "../block/Block.hpp"
#include <set>
#include <string>
#include <vector>

class Script {
public:
  int attached_to_sprite_id = -1;
  std::vector<Block> blocks;

  Script() { blocks.reserve(16); }

  std::set<std::string> get_names_of_all_blocks() {
    // Get name of all the blocks (non-repeated).
    std::set<std::string> names;
    for (const auto &block : blocks) {
      names.insert(block.function_identifier);
    }

    return names;
  }
};

#endif // SCRIPT_HPP
