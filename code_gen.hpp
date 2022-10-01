#ifndef CODE_GEN_HPP
#define CODE_GEN_HPP

#include "block/Block.hpp"
#include <fstream>
#include <string>

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

  if (message == "" || message.empty()) {
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

  std::string code = "";
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

  std::string code = "";
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

  std::string code = "";
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

  std::string code = "";
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

void generate_code(std::vector<Block> &blocks,
                   const std::string &default_sprite_name,
                   const std::string &sprite_file_name,
                   const sf::Vector2f sprite_initial_position) {

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

  auto spr_init_code = std::string(
      "sf::Texture ##SPRITE_NAME##_texture;"
      "if (!##SPRITE_NAME##_texture.loadFromFile(\"" +
      sprite_file_name +
      "\")) {"
      "   std::cerr << \"Error while loading texture\" << std::endl;"
      "   return -1;"
      " }"
      "##SPRITE_NAME##_texture.setSmooth(true);\n\n"
      "sf::Sprite ##SPRITE_NAME##;"
      "##SPRITE_NAME##.setTexture(##SPRITE_NAME##_texture);      "
      "sf::FloatRect ##SPRITE_NAME##Size = "
      "##SPRITE_NAME##.getGlobalBounds();        "
      "##SPRITE_NAME##.setOrigin(##SPRITE_NAME##Size.width / 2., "
      "##SPRITE_NAME##Size.height / 2.); "
      "##SPRITE_NAME##.setPosition(" +
      std::to_string((int)sprite_initial_position.x) + "," +
      std::to_string((int)sprite_initial_position.y) + ");");

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

#endif
