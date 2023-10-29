#include "CodeGenerator.hpp"
#include "../Nodes/SpriteNode.hpp"
#include "../Utils.hpp"

using namespace STRING_UTILITIES;

void CodeGenerator::_substitute_sprite_name(std::string &code,
                                            const std::string &sprite_name) {
  replaceAll(code, "##SPRITE_NAME##", sprite_name);
}

std::string CodeGenerator::_construct_sprite_code(const EditorSprite &spr) {
  auto sprite_name = spr.name;
  auto sprite_pos = spr.position;
  auto sprite_texture_file = spr.texture;

  std::string spr_cons_code = "";

#define OBJECT_IS(T) auto casted = dynamic_cast<T *>(spr.get_node())
  if (OBJECT_IS(SpriteNode)) {

    auto sprite_scale = casted->get_shape().getScale();

    spr_cons_code =
        "sf::Texture ##SPRITE_NAME##_texture;\n"
        "if (!##SPRITE_NAME##_texture.loadFromFile(\"" +
        sprite_texture_file +
        "\")) {\n"
        " std::cerr << \"Error while loading texture\" << std::endl;\n"
        " return -1;\n"
        "}\n"
        "##SPRITE_NAME##_texture.setSmooth(true);\n\n"
        "sf::Sprite ##SPRITE_NAME##;\n"
        "##SPRITE_NAME##.setTexture(##SPRITE_NAME##_texture);\n"
        "sf::FloatRect ##SPRITE_NAME##Size = "
        "##SPRITE_NAME##.getGlobalBounds();\n"
        "##SPRITE_NAME##.setOrigin(##SPRITE_NAME##Size.width / 2.0f, "
        "##SPRITE_NAME##Size.height / 2.0f);\n"
        "##SPRITE_NAME##.setPosition(" +
        std::to_string(static_cast<int>(sprite_pos.x)) + "," +
        std::to_string(static_cast<int>(sprite_pos.y)) + ");\n" +
        "##SPRITE_NAME##.setScale(" + std::to_string(sprite_scale.x) + "," +
        std::to_string(sprite_scale.y) + ");\n";

    _substitute_sprite_name(spr_cons_code, sprite_name);
  }
#undef OBJECT_IS

  return spr_cons_code;
}

std::string CodeGenerator::_render_sprite_code(const std::string &sprite_name) {
  std::string spr_render_code = "window.draw(##SPRITE_NAME##);";
  _substitute_sprite_name(spr_render_code, sprite_name);
  return spr_render_code;
}

std::string CodeGenerator::_get_update_player_velocity_code() {
  std::string code =
      ""
      "##SPRITE_NAME##__velocity = {0.0f, 0.0f};\n"
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {\n"
      "  ##SPRITE_NAME##__velocity.x += 1.0f;\n"
      "}\n"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {\n"
      "  ##SPRITE_NAME##__velocity.x -= 1.0f;\n"
      "}\n"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {\n"
      "  ##SPRITE_NAME##__velocity.y += 1.0f;\n"
      "}\n"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {\n"
      "  ##SPRITE_NAME##__velocity.y -= 1.0f;\n"
      "}\n\n"
      ""
      "int ##SPRITE_NAME##__speed = 200;\n"
      "##SPRITE_NAME##__velocity = normalized(##SPRITE_NAME##__velocity);\n"
      "##SPRITE_NAME##__velocity.x *= ##SPRITE_NAME##__speed;\n"
      "##SPRITE_NAME##__velocity.y *= ##SPRITE_NAME##__speed;\n"
      "##SPRITE_NAME##.move(##SPRITE_NAME##__velocity * "
      "deltaTime.asSeconds());\n\n";
  return code;
}

std::string CodeGenerator::construct_code() {
  std::string cons_code;
  for (const auto &spr : editor_ref.user_added_sprites) {
    cons_code += _construct_sprite_code(spr);

    // Hacky Functions to add Movement scripts to the sprites.
    if (spr.add_movement_script) {
      auto sprite_name = spr.name;
      // sf::Vector2f velocity;
      cons_code += "\nsf::Vector2f " + sprite_name + "__velocity;\n";
    }
    // Hacky Functions End.

    cons_code += "\n\n";
  }

  return cons_code;
}

std::string CodeGenerator::render_all_sprites_code() {
  // Write Render Code, so that they draw sorted by their layers.
  auto sorted_sprites = editor_ref.get_sprites_sorted_by_layers();

  std::string render_code;
  for (const auto &spr : sorted_sprites) {
    auto sprite_name = spr->name;
    render_code += _render_sprite_code(sprite_name);
    render_code += "\n";
  }

  return render_code;
}

std::string CodeGenerator::main_loop_code() {
  std::string main_loop_code;

  for (auto &spr : editor_ref.user_added_sprites) {
    auto *script = editor_ref.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    // Hacky Functions to add Movement scripts to the sprites.
    if (spr.add_movement_script) {
      main_loop_code += "\n" + _get_update_player_velocity_code();
      _substitute_sprite_name(main_loop_code, sprite_name);
    }
    // Hacky Functions End.

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      if (!BLOCK_UTILITIES::is_forever_block(block)) {
        continue;
      }

      auto output_code = block.get_code();

      // The generated code should be in the main loop.
      std::cout << "[MainLoopBlock]\n";
      // Remove the braces from the generated code, to get the actual code
      // to write.
      remove_first_occurence(output_code, '{');
      remove_last_occurence(output_code, '}');

      main_loop_code += output_code;

      _substitute_sprite_name(main_loop_code, sprite_name);
    }
  }

  return main_loop_code;
}

std::string CodeGenerator::input_code() {
  std::string input_code;

  for (auto &spr : editor_ref.user_added_sprites) {
    auto *script = editor_ref.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      if (!BLOCK_UTILITIES::is_input_block(block)) {
        continue;
      }

      auto output_code = block.get_code();
      input_code += output_code;
      input_code += "}\n";

      _substitute_sprite_name(input_code, sprite_name);
    }
  }

  return input_code;
}

std::string CodeGenerator::when_program_starts_code() {
  // The generated code should be before the main loop of the generated output
  // code.
  std::string init_code;

  for (auto &spr : editor_ref.user_added_sprites) {
    auto *script = editor_ref.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      if (!BLOCK_UTILITIES::is_program_starts_block(block)) {
        continue;
      }

      auto output_code = block.get_code();
      init_code += output_code;

      _substitute_sprite_name(init_code, sprite_name);
    }
  }

  return init_code;
}

std::string CodeGenerator::blocks_init_code() {
  std::string init_code;

  for (auto &spr : editor_ref.user_added_sprites) {
    auto *script = editor_ref.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    for (auto &block : script->blocks) {
      init_code += block.get_code_for_init();
      _substitute_sprite_name(init_code, sprite_name);
    }
  }

  return init_code;
}

std::set<std::string> CodeGenerator::get_all_modules_required() {
  // Multiple scripts can have multiple blocks of same name.
  // So collect them all at once in a set(), so that we have non-repeated
  // names.
  std::set<std::string> block_names;

  for (auto &spr : editor_ref.user_added_sprites) {
    auto *script = editor_ref.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto names = script->get_names_of_all_blocks();
    block_names.insert(names.begin(), names.end());
  }

  // Get all the modules required for those block names.
  using FUNCTION_IDENTIFIER = std::string;
  using MODULE_REQUIRED = std::string;

  const std::vector<std::pair<FUNCTION_IDENTIFIER, MODULE_REQUIRED>> db = {
      {"block_sprite_clicked", "IS_MOUSE_OVER"},
      {"block_sprite_touching", "ARE_SPRITES_COLLIDING"},
      {"block_default_character_controller", "CHARACTER_MOVEMENT"},
      {"block_say", "BUBBLE_TEXT"},
      {"block_draw_text", "DRAW_TEXT"},
      {"block_change_x_by_in", "MOVE_TO_POINT"},
      {"block_change_y_by_in", "MOVE_TO_POINT"},
      {"block_glide_to_xy", "MOVE_TO_POINT"},
      {"block_glide_point_to_point", "MOVE_POINT_TO_POINT"}};

  std::set<std::string> modules;
  for (const auto &name : block_names) {
    for (auto &[fn_identifier, module] : db) {
      if (fn_identifier == name) {
        modules.insert(module);
      }
    }
  }

  return modules;
}

void CodeGenerator::generate_code() {
  auto init_code = construct_code() + "\n";
  init_code += when_program_starts_code() + "\n";
  init_code += blocks_init_code();

  auto _main_loop_code = main_loop_code();
  auto _input_code = input_code();
  auto render_code = render_all_sprites_code();

  auto required_modules = get_all_modules_required();
  std::cout << "The modules required for the code export are: \n";
  for (const auto &mod : required_modules) {
    std::cout << mod << "\n";
  }

  // All these above code generations can be done in a single loop.
  // But this seems more clean.
  // See 'Single Loop Implementation' below.
  std::ifstream code_template("CodeGen\\template.cpp");
  std::string template_code((std::istreambuf_iterator<char>(code_template)),
                            (std::istreambuf_iterator<char>()));

  Preprocessor::pre_process(template_code, required_modules);

  replaceAll(template_code, "//###INIT_CODES###", init_code);
  replaceAll(template_code, "//###MAINLOOP_CODE###", _main_loop_code);
  replaceAll(template_code, "//###INPUT_CODE###", _input_code);
  replaceAll(template_code, "//###RENDER_CODE###", render_code);

  std::ofstream generated_file("E:\\Sclone 2.0\\GeneratedOutput\\main.cpp");
  generated_file << template_code;
  generated_file.close();

  std::cout << "Code Generated.\n";
}

/*
Single Loop Implementation for Code Generation For Future Reference.
void generate_code(std::vector<Block> &blocks,
                   const std::string &default_sprite_name,
                   const std::string &sprite_file_name,
                   const sf::Vector2f sprite_initial_position,
                   const std::string &p_init_code) {

  std::string init_code = "";
  std::string main_loop_code = "";
  std::string spr_render_code = "window.draw(##SPRITE_NAME##);";
  std::string input_code = "";

  std::cout << "\n\n[Generated Code]\n";
  for (auto &block : blocks) {
    if (!block.is_control_block()) {
      continue;
    }

    auto output_code = block.get_code();

    // EDITOR SHORCUT MACROS:
    //  Shortcut in the editor to get position of the current sprite.
    //  This applies to code everywhere, so we perform the replace operation
    //  right here.
    replaceAll(output_code, "#POS#", "##SPRITE_NAME##.getPosition()");
    replaceAll(output_code, "#WIN_W#", "width");
    replaceAll(output_code, "#WIN_H#", "height");
    replaceAll(output_code, "#M_X#", "sf::Mouse::getPosition(window).x");
    replaceAll(output_code, "#M_Y#", "sf::Mouse::getPosition(window).y");

    // The first child of "Forever" Block has text "Forever".
    bool is_forever_block =
        std::any_of(block.childrens.begin(), block.childrens.end(),
                    [](std::shared_ptr<NODEBaseClass> b) {
                      return b->get_text() == "Forever";
                    });

    bool is_input_block =
        std::any_of(block.childrens.begin(), block.childrens.end(),
                    [](std::shared_ptr<NODEBaseClass> b) {
                      return b->get_text() == "When It's Clicked";
                    });

    if (is_forever_block) {
      // The generated code should be in the main loop.
      std::cout << "[MainLoopBlock]\n";
      // Remove the braces from the generated code, to get the actual code to
      // write.
      remove_first_occurence(output_code, '{');
      remove_last_occurence(output_code, '}');

      main_loop_code += output_code;
    } else if (is_input_block) {
      input_code += output_code;
      input_code += "}\n";
    } else {
      // The generated code should be before the main loop.
      init_code += output_code;
    }

    std::cout << output_code;
  }

  std::ifstream code_template("template.cpp");
  std::string template_code((std::istreambuf_iterator<char>(code_template)),
                            (std::istreambuf_iterator<char>()));

  auto spr_init_code = p_init_code;
  spr_init_code += "\n" + init_code;

  replaceAll(template_code, "//###INIT_CODES###", spr_init_code);
  replaceAll(template_code, "//###MAINLOOP_CODE###", main_loop_code);
  replaceAll(template_code, "//###RENDER_CODE###", spr_render_code);
  replaceAll(template_code, "//###INPUT_CODE###", input_code);
  replaceAll(template_code, "##SPRITE_NAME##", default_sprite_name);

  std::ofstream generated_file("E:\\Sclone 2.0\\GeneratedOutput\\main.cpp");
  generated_file << template_code;
  generated_file.close();
}
*/

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

namespace Preprocessor {
void pre_process(std::string &code,
                 const std::set<std::string> &required_modules) {

  auto is_this_module_required = [&required_modules](const std::string &p_mod) {
    return required_modules.find(p_mod) != required_modules.end();
  };

  using index_position = unsigned int;
  using string_length = std::size_t;
  // Pair of 'position' and 'length' of the entire module code to strip away.
  std::vector<std::pair<index_position, string_length>> strip_pos_length_pair;

  // In 'template.cpp':
  // A module is defined as :
  // MODULE : "BUBBLE_TEXT" //

  const std::string module_def_str = "// MODULE : ";

  std::string::size_type pos = 0;
  // Find all the positions which starts with module_def_str.
  while ((pos = code.find(module_def_str, pos)) != std::string::npos) {
    pos += module_def_str.length();

    //  // MODULE : "BUBBLE_TEXT" //
    //              ^ pos
    //                           ^ module_name_end_pos

    // Module defination has // in the end.
    auto module_name_end_pos = code.find(" //", pos);

    // Get the name of module which is in between the strings  "// MODULE : "
    // and " //".
    auto module_name = code.substr(pos, module_name_end_pos - pos);

    // Module name obtained is : "BUBBLE_TEXT".
    // Remove the double quotes to get the actual string.
    remove_first_occurence(module_name, '\"');
    remove_last_occurence(module_name, '\"');

    bool keep_module = is_this_module_required(module_name);

    //  // MODULE : "BUBBLE_TEXT" //
    //              ^ pos
    //  ^ mod_start_pos
    auto mod_start_pos = pos - module_def_str.length();

    if (keep_module) {
      // The module should be in the final generated code.
      // However the module code contains all these preprocessor commands.
      // Strip the preprocessor commands away to make it look good, however the
      // actual module body is kept.

      //  // MODULE : "BUBBLE_TEXT" //
      //                          ^ module_name_end_pos
      //  ^ mod_start_pos             ^mod_def_end

      auto mod_def_end = module_name_end_pos + std::string(" //").length();
      auto mod_def_line_length = mod_def_end - mod_start_pos;

      strip_pos_length_pair.insert(strip_pos_length_pair.begin(),
                                   {mod_start_pos, mod_def_line_length});
    } else {
      // The required module shouldn't be in the final generated code.

      // A module defination ends with "// MODULE_END //".
      const std::string module_end_str = "// MODULE_END //";
      // // MODULE_END //".
      //                ^ module_def_end_pos
      auto module_def_end_pos =
          code.find(module_end_str, pos) + module_end_str.length();

      auto length_of_full_module_code = module_def_end_pos - mod_start_pos;

      // Add the 'position of start of the module code' and 'entire length of
      // the module code' into the beginning of a vector as a pair.
      // In this way, modules at the last of the template code appears in the
      // first of the vector, so we can erase the strings from behind without
      // messing the positions of strings.
      strip_pos_length_pair.insert(strip_pos_length_pair.begin(),
                                   {mod_start_pos, length_of_full_module_code});
    }
  }

  // From above, these are the module codes that shouldn't be in the final
  // generated code.
  // So strip all them.
  for (auto [pos, len] : strip_pos_length_pair) {
    code.erase(pos, len);
  }

  // This should be below the loop, so it doesn't mess up the position of
  // strings.
  replaceAll(code, "// MODULE_END //", "");
}

} // namespace Preprocessor
