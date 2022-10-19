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

  std::set<std::string> get_all_modules_required() {
    using FUNCTION_IDENTIFIER = std::string;
    using MODULE_REQUIRED = std::string;

    const std::vector<std::pair<FUNCTION_IDENTIFIER, MODULE_REQUIRED>> db = {
        {"block_sprite_clicked", "IS_MOUSE_OVER"},
        {"block_sprite_touching", "ARE_SPRITES_COLLIDING"},
        {"block_default_character_controller", "CHARACTER_MOVEMENT"},
        {"block_say", "BUBBLE_TEXT"},
        {"block_glide_to_xy", "MOVE_TO_POINT"},
        {"block_glide_point_to_point", "MOVE_POINT_TO_POINT"}};

    std::set<std::string> modules;

    auto add_modules_for_a_block = [&modules, &db](const Block &block) {
      for (auto &[fn_identifier, module] : db) {
        if (block.function_identifier == fn_identifier) {
          modules.insert(module);
        }
      }
    };

    for (const auto &block : blocks) {
      add_modules_for_a_block(block);
    }

    return modules;
  }
};

#endif // SCRIPT_HPP
