#ifndef CODE_GEN_HPP
#define CODE_GEN_HPP

#include "../Editor/Editor.hpp"
#include "../block/Block.hpp"
#include "Preprocessor.hpp"
#include <fstream>
#include <set>
#include <string>

std::string code_default_character_controller(const Block &block) {
  auto speed = block.get_bound_value("speed").value();

  if (speed.empty()) {
    speed = "200";
  }

  return "add_character_movement(##SPRITE_NAME##, deltaTime," + speed + ");\n";
}

std::string code_sprite_set_position(const Block &block) {
  //"x" -> block.childrens.at(2).text
  //"y" -> block.childrens.at(4).text
  auto x_pos = block.get_bound_value("x").value();
  auto y_pos = block.get_bound_value("y").value();
  return "##SPRITE_NAME##.setPosition(" + x_pos + ", " + y_pos + ");";
}

std::string code_sprite_glide_to_xy(const Block &block) {
  auto x_pos = block.get_bound_value("x").value();
  auto y_pos = block.get_bound_value("y").value();
  auto length = block.get_bound_value("length").value();

  auto target_pos_str = "sf::Vector2f(" + x_pos + ", " + y_pos + ")";

  return "add_move_to_point_operation(&##SPRITE_NAME##," + target_pos_str +
         "," + length + ");";
}

std::string code_sprite_glide_point_to_point(const Block &block) {
  auto x_pos = block.get_bound_value("x").value();
  auto y_pos = block.get_bound_value("y").value();
  auto length = block.get_bound_value("length").value();
  auto x1_pos = block.get_bound_value("x1").value();
  auto y1_pos = block.get_bound_value("y1").value();

  auto current_pos_str = "sf::Vector2f(" + x_pos + ", " + y_pos + ")";
  auto target_pos_str = "sf::Vector2f(" + x1_pos + ", " + y1_pos + ")";

  return "add_move_p2p_operation(&##SPRITE_NAME##," + current_pos_str + "," +
         target_pos_str + "," + length + ");";
}

std::string code_sprite_forever(const Block &block) {
  return "";
} //"for(;;)"; }

std::string code_sprite_if_else(const Block &block) {
  auto condition = block.get_bound_value("condition").value();
  return "if(" + condition + ")";
}

std::string code_sprite_say(const Block &block) {
  auto message = block.get_bound_value("message").value();
  auto message_visible_length = block.get_bound_value("length").value();

  if (message_visible_length.empty()) {
    message_visible_length = "1.0f";
  }

  return "add_bubble_message(&##SPRITE_NAME##," + message_visible_length +
         ", \"" + message + "\");";
}

std::string code_sprite_change_x_by(const Block &block) {
  auto x_offset = block.get_bound_value("x_offset").value();
  return "##SPRITE_NAME##.move(sf::Vector2f(" + x_offset + ", 0.0f));";
}

std::string code_sprite_change_y_by(const Block &block) {
  auto y_offset = block.get_bound_value("y_offset").value();
  return "##SPRITE_NAME##.move(sf::Vector2f(0.0f, " + y_offset + "));";
}

std::string code_input_key_pressed(const Block &block) {
  auto key = block.get_bound_value("selected_key").value();
  return "if(sf::Keyboard::isKeyPressed(sf::Keyboard::" + key + "))";
}

std::string code_sprite_touching(const Block &block) {
  auto target_sprite = block.get_bound_value("target_sprite").value();
  return "if (are_sprites_colliding(##SPRITE_NAME##, " + target_sprite + "))";
}

std::string code_sprite_clicked(const Block &block) {
  return "if (e.type == sf::Event::MouseButtonReleased &&"
         "e.mouseButton.button == sf::Mouse::Left && "
         "is_mouse_over(&##SPRITE_NAME##)){";
}

// In these primitive renderers,
// We create a scope to avoid multiple definations for same sprite drawn
// multiple times using the same name.
std::string code_sprite_draw_line(const Block &block) {
  /*
    //Draw a line.
    sf::Vertex vertices[2] = {
        sf::Vertex({50.0,50.0}, sf::Color::Red),
        sf::Vertex({500.0,500.0}, sf::Color::Green)
    };

    window.draw(vertices, 2, sf::Lines);
  */
  auto start_pos = block.get_bound_value("starting_pos").value();
  auto end_pos = block.get_bound_value("ending_pos").value();

  std::string code;
  code += "{ \n";
  code += "   sf::Vertex vertices[2] = {";
  code += "       sf::Vertex({" + start_pos + "}, sf::Color::Red),";
  code += "       sf::Vertex({" + end_pos + "}, sf::Color::Green)";
  code += "   }; \n";
  code += "   window.draw(vertices, 2, sf::Lines);\n";
  code += "} \n";

  return code;
}

std::string code_sprite_draw_circle(const Block &block) {
  /*
    //Draw a circle.
    sf::CircleShape circle;
    circle.setPosition({50.0,50.0});
    circle.setRadius(10.0f);

    window.draw(circle);
  */
  auto centre = block.get_bound_value("centre").value();
  auto radius = block.get_bound_value("radius").value();

  std::string code;
  code += "{ \n";
  code += "   sf::CircleShape circle;\n";
  code += "   circle.setPosition({" + centre + "});\n";
  code += "   circle.setRadius(" + radius + ");\n";
  code += "   window.draw(circle);\n";
  code += "} \n";

  return code;
}

std::string code_sprite_draw_rectangle(const Block &block) {
  /*
    //Draw a rectangle.
    sf::RectangleShape rectangle;
    rectangle.setPosition({50.0,50.0});
    rectangle.setSize({200.0f, 200.0f});

    window.draw(rectangle);
  */
  auto top_left = block.get_bound_value("top_left").value();
  auto rect_size = block.get_bound_value("rect_size").value();

  std::string code;
  code += "{ \n";
  code += "   sf::RectangleShape rectangle;\n";
  code += "   rectangle.setPosition({" + top_left + "});\n";
  code += "   rectangle.setSize({" + rect_size + "});\n";
  code += "   window.draw(rectangle);\n";
  code += "} \n";

  return code;
}

std::string code_sprite_draw_triangle(const Block &block) {
  /*
  //Draw A triangle.
  // https://www.sfml-dev.org/tutorials/2.5/graphics-shape.php

  //  create an empty shape
  sf::ConvexShape convex;

  // resize it to 3 points
  convex.setPointCount(3);

  // define the points
  convex.setPoint(0, sf::Vector2f(0.f, 0.f));
  convex.setPoint(1, sf::Vector2f(150.f, 10.f));
  convex.setPoint(2, sf::Vector2f(120.f, 90.f));
  */
  auto first_point = block.get_bound_value("first_point").value();
  auto second_point = block.get_bound_value("second_point").value();
  auto third_point = block.get_bound_value("third_point").value();

  std::string code;
  code += "{ \n";
  code += "   sf::ConvexShape triangle;\n";
  code += "   triangle.setPointCount(3);\n";

  code += "   triangle.setPoint(0,{" + first_point + "});\n";
  code += "   triangle.setPoint(1,{" + second_point + "});\n";
  code += "   triangle.setPoint(2,{" + third_point + "});\n\n";
  code += "   window.draw(triangle);\n";
  code += "} \n";

  return code;
}

// 'Create' Blocks are to Create Variables.
// 'Set' Blocks are to Update the already created Variables.

// We use underscores before sprite names, so that the variables don't collide
// with variables of other sprites. As of now, all the variables are in the same
// namespace, so.

std::string code_set_int(const Block &block) {
  auto int_name = block.get_bound_value("variable_name").value();
  auto int_value = block.get_bound_value("variable_value").value();
  return "_##SPRITE_NAME##_" + int_name + " = " + int_value + ";";
}

std::string code_create_int(const Block &block) {
  return "int " + code_set_int(block);
  // Same As:
  //  auto int_name = block.get_bound_value("variable_name").value();
  //  auto int_value = block.get_bound_value("variable_value").value();
  //  return "int _##SPRITE_NAME##_" + int_name + " = " + int_value + ";";
}

std::string code_set_float(const Block &block) {
  auto float_name = block.get_bound_value("variable_name").value();
  auto float_value = block.get_bound_value("variable_value").value();
  return "_##SPRITE_NAME##_" + float_name + " = " + float_value + ";";
}

std::string code_create_float(const Block &block) {
  return "float " + code_set_float(block);
  // Same As:
  // auto float_name = block.get_bound_value("variable_name").value();
  // auto float_value = block.get_bound_value("variable_value").value();
  // return "float _##SPRITE_NAME##_" + float_name + " = " + float_value + ";";
}

std::string code_set_string(const Block &block) {
  auto string_name = block.get_bound_value("variable_name").value();
  auto string_value = block.get_bound_value("variable_value").value();
  return "_##SPRITE_NAME##_" + string_name + " = {\"" + string_value + "\"};";
}

std::string code_create_string(const Block &block) {
  return "std::string " + code_set_string(block);
  // Same As:
  // auto string_name = block.get_bound_value("variable_name").value();
  // auto string_value = block.get_bound_value("variable_value").value();
  // return "std::string _##SPRITE_NAME##_" + string_name + "{\"" + string_value
  // +
  //  "\"};";
}

std::string code_set_vector2f(const Block &block) {
  auto vector2f_name = block.get_bound_value("variable_name").value();
  auto vector2f_value_x = block.get_bound_value("variable_value_x").value();
  auto vector2f_value_y = block.get_bound_value("variable_value_y").value();

  // If values are left empty, then use the previous values.
  // TODO? Implement this for other variable types as well ??

  if (vector2f_value_x.empty()) {
    if (vector2f_value_y.empty()) {
      std::cout << "[Code Modification] Both \"x\" and \"y\" field of the "
                   "vector2f was left "
                   "empty. So stripping away the set code.\n";
      return "\n";
    } else {
      std::cout << "[Code Modification] \"x\" field of the vector2f was left "
                   "empty. So just setting the \"y\".\n";
      return "_##SPRITE_NAME##_" + vector2f_name + ".y = " + vector2f_value_y +
             ";";
    }
  }

  if (vector2f_value_y.empty()) {
    if (!vector2f_value_x.empty()) {
      std::cout << "[Code Modification] \"y\" field of the vector2f was left "
                   "empty. So just setting the \"x\".\n";
      return "_##SPRITE_NAME##_" + vector2f_name + ".x = " + vector2f_value_x +
             ";";
    }
    // Else case of the inner loop is when both the fields are empty and is
    // handled above already.
  }

  // Both the values are set.
  return "_##SPRITE_NAME##_" + vector2f_name + "= {" + vector2f_value_x + "," +
         vector2f_value_y + "};";
}

std::string code_create_vector2f(const Block &block) {
  auto vector2f_name = block.get_bound_value("variable_name").value();
  auto vector2f_value_x = block.get_bound_value("variable_value_x").value();
  auto vector2f_value_y = block.get_bound_value("variable_value_y").value();
  // {} initialization.
  return "sf::Vector2f _##SPRITE_NAME##_" + vector2f_name + " {" +
         vector2f_value_x + "," + vector2f_value_y + "};";
}

void substitute_sprite_name(std::string &code, const std::string &sprite_name) {
  replaceAll(code, "##SPRITE_NAME##", sprite_name);
}

std::string construct_sprite_code(const EditorSprite &spr) {
  auto sprite_name = spr.name;
  auto sprite_pos = spr.position;
  auto sprite_texture_file = spr.texture;

  std::string spr_cons_code =
      "sf::Texture ##SPRITE_NAME##_texture;"
      "if (!##SPRITE_NAME##_texture.loadFromFile(\"" +
      sprite_texture_file +
      "\")) {"
      "   std::cerr << \"Error while loading texture\" << std::endl;"
      "   return -1;"
      " }"
      "##SPRITE_NAME##_texture.setSmooth(true);\n\n"
      "sf::Sprite ##SPRITE_NAME##;"
      "##SPRITE_NAME##.setTexture(##SPRITE_NAME##_texture);      "
      "sf::FloatRect ##SPRITE_NAME##Size = "
      "##SPRITE_NAME##.getGlobalBounds();        "
      "##SPRITE_NAME##.setOrigin(##SPRITE_NAME##Size.width / 2.0f, "
      "##SPRITE_NAME##Size.height / 2.0f); "
      "##SPRITE_NAME##.setPosition(" +
      std::to_string(static_cast<int>(sprite_pos.x)) + "," +
      std::to_string(static_cast<int>(sprite_pos.y)) + ");";

  substitute_sprite_name(spr_cons_code, sprite_name);
  return spr_cons_code;
}

std::string render_sprite_code(const std::string &sprite_name) {
  std::string spr_render_code = "window.draw(##SPRITE_NAME##);";
  substitute_sprite_name(spr_render_code, sprite_name);
  return spr_render_code;
}

std::string _get_update_player_velocity_code() {
  std::string code =
      ""
      "##SPRITE_NAME##__velocity = {0.0f, 0.0f};"
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {"
      "  ##SPRITE_NAME##__velocity.x += 1.0f;"
      "}"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {"
      "  ##SPRITE_NAME##__velocity.x -= 1.0f;"
      "}"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {"
      "  ##SPRITE_NAME##__velocity.y += 1.0f;"
      "}"
      ""
      "if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {"
      "  ##SPRITE_NAME##__velocity.y -= 1.0f;"
      "}\n\n"
      ""
      "int ##SPRITE_NAME##__speed = 200;"
      "##SPRITE_NAME##__velocity = normalized(##SPRITE_NAME##__velocity);"
      "##SPRITE_NAME##__velocity.x *= ##SPRITE_NAME##__speed;"
      "##SPRITE_NAME##__velocity.y *= ##SPRITE_NAME##__speed;"
      "##SPRITE_NAME##.move(##SPRITE_NAME##__velocity * "
      "deltaTime.asSeconds());\n"
      "";
  return code;
}

std::string _construct_code(const Editor &editor) {
  std::string cons_code;
  for (const auto &spr : editor.user_added_sprites) {
    cons_code += construct_sprite_code(spr);

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

std::string _render_all_sprites_code(const Editor &editor) {
  /*
  std::string render_code;
  for (const auto &spr : editor.user_added_sprites) {
    auto sprite_name = spr.name;
    render_code += render_sprite_code(sprite_name);
    render_code += "\n";
  }

  return render_code;
 */

  // Write Render Code, so that they draw sorted by their layers.
  auto sorted_sprites = editor.get_sprites_sorted_by_layers();

  std::string render_code;
  for (const auto &spr : sorted_sprites) {
    auto sprite_name = spr->name;
    render_code += render_sprite_code(sprite_name);
    render_code += "\n";
  }

  return render_code;
}

std::string _main_loop_code(Editor &editor) {
  std::string main_loop_code;

  for (auto &spr : editor.user_added_sprites) {
    auto *script = editor.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    // Hacky Functions to add Movement scripts to the sprites.
    if (spr.add_movement_script) {
      main_loop_code += "\n" + _get_update_player_velocity_code();
      substitute_sprite_name(main_loop_code, sprite_name);
    }
    // Hacky Functions End.

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      // The first child of "Forever" Block has text "Forever".
      bool is_forever_block =
          std::any_of(block.childrens.begin(), block.childrens.end(),
                      [](std::shared_ptr<NODEBaseClass> p_block) {
                        return p_block->get_text() == "Forever";
                      });
      if (!is_forever_block) {
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

      substitute_sprite_name(main_loop_code, sprite_name);
    }
  }

  return main_loop_code;
}

std::string _input_code(Editor &editor) {
  std::string input_code;

  for (auto &spr : editor.user_added_sprites) {
    auto *script = editor.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      bool is_input_block =
          std::any_of(block.childrens.begin(), block.childrens.end(),
                      [](std::shared_ptr<NODEBaseClass> p_block) {
                        return p_block->get_text() == "When It's Clicked";
                      });
      if (!is_input_block) {
        continue;
      }

      auto output_code = block.get_code();
      input_code += output_code;
      input_code += "}\n";

      substitute_sprite_name(input_code, sprite_name);
    }
  }

  return input_code;
}

std::string _when_program_starts_code(Editor &editor) {
  // The generated code should be before the main loop of the generated output
  // code.
  std::string init_code;

  for (auto &spr : editor.user_added_sprites) {
    auto *script = editor.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto sprite_name = spr.name;

    for (auto &block : script->blocks) {
      if (!block.is_control_block()) {
        continue;
      }

      bool is_program_starts_block =
          std::any_of(block.childrens.begin(), block.childrens.end(),
                      [](std::shared_ptr<NODEBaseClass> p_block) {
                        return p_block->get_text() == "When Program Starts";
                      });
      if (!is_program_starts_block) {
        continue;
      }

      auto output_code = block.get_code();
      init_code += output_code;

      substitute_sprite_name(init_code, sprite_name);
    }
  }

  return init_code;
}

std::set<std::string> get_all_modules_required(Editor &editor) {
  std::set<std::string> all_modules;

  for (auto &spr : editor.user_added_sprites) {
    auto *script = editor.get_script_attached_to_editor_sprite(&spr);
    if (script == nullptr) {
      continue;
    }

    auto spr_modules = script->get_all_modules_required();
    all_modules.insert(spr_modules.begin(), spr_modules.end());
  }

  return all_modules;
}

void generate_code(Editor &editor) {
  auto init_code = _construct_code(editor) + "\n";
  init_code += _when_program_starts_code(editor);

  auto main_loop_code = _main_loop_code(editor);
  auto input_code = _input_code(editor);
  auto render_code = _render_all_sprites_code(editor);

  auto required_modules = get_all_modules_required(editor);
  std::cout << "The modules required for the code export are: \n";
  for (auto &mod : required_modules) {
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
  replaceAll(template_code, "//###MAINLOOP_CODE###", main_loop_code);
  replaceAll(template_code, "//###INPUT_CODE###", input_code);
  replaceAll(template_code, "//###RENDER_CODE###", render_code);

  std::ofstream generated_file("E:\\Sclone 2.0\\GeneratedOutput\\main.cpp");
  generated_file << template_code;
  generated_file.close();
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

#endif
