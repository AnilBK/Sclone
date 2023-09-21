#ifndef OUTPUT_CODE_CALL_BACKS_HPP
#define OUTPUT_CODE_CALL_BACKS_HPP

#include "../block/Block.hpp"
#include "FormatStrings.hpp"
#include <string>

// Certain Block generates some special code during code generation.
// The code generation for those blocks are implemented below.
// These functions are referenced by the blocks are their
// 'output_code_callback'. So when a block wants to generate code for it, the
// block calls these callbacks.

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

std::string code_sprite_move_by_vector2(const Block &block) {
  auto offset = block.get_bound_value("offset").value();
  return "##SPRITE_NAME##.move(_##SPRITE_NAME##_" + offset + ");";
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

std::string code_draw_text(const Block &block) {
  /*
  //https: // www.sfml-dev.org/tutorials/2.5/graphics-text.php
  sf::Text text;

  // select the font
  text.setFont(font); // font is a sf::Font

  // set the string to display
  text.setString("Hello world");

  // set the character size
  text.setCharacterSize(24); // in pixels, not points!

  // set the color
  text.setFillColor(sf::Color::Red);

  // set the text style
  text.setStyle(sf::Text::Bold | sf::Text::Underlined);

  ...

  // inside the main loop, between window.clear() and window.display()
  window.draw(text);
  */

  auto string = block.get_bound_value("string").value();

  std::string code;
  if (fStrings::is_fString(string)) {
    // fStrings have variables, so they are set up in the main loop instead.
    auto formatted_string_code = fStrings::formatted_string_to_code(string);
    code += "##SPRITE_NAME##__text.setString(" + formatted_string_code + ");\n";
  }
  code += "window.draw(##SPRITE_NAME##__text);";
  return code;
}

std::string code_draw_text_init(const Block &block) {
  auto x = block.get_bound_value("x").value();
  auto y = block.get_bound_value("y").value();
  auto string = block.get_bound_value("string").value();

  std::string code;
  code += "sf::Text ##SPRITE_NAME##__text;";
  code += "##SPRITE_NAME##__text.setPosition({" + x + " , " + y + " });";
  code += "##SPRITE_NAME##__text.setFont(text_font);";
  code += "##SPRITE_NAME##__text.setCharacterSize(24);";
  code += "##SPRITE_NAME##__text.setFillColor(sf::Color::Red);";

  // fStrings have variables, so they can't be set up during initialization.
  // They should be set up in main loop instead.
  if (!fStrings::is_fString(string)) {
    code += "##SPRITE_NAME##__text.setString(\"" + string + "\");";
  }

  return code;
}

std::string code_alternate_between_init(const Block &block) {
  std::string code;
  code += "bool ##SPRITE_NAME##__flip_flop = true;";
  return code;
}

std::string code_alternate_between(const Block &block) {
  return "if(##SPRITE_NAME##__flip_flop)";
}

std::string code_sprite_change_x_by(const Block &block) {
  auto x_offset = block.get_bound_value("x_offset").value();
  return "##SPRITE_NAME##.move(sf::Vector2f(" + x_offset + ", 0.0f));";
}

std::string code_sprite_change_x_by_in(const Block &block) {
  auto x_offset = block.get_bound_value("x_offset").value();
  auto length = block.get_bound_value("length").value();

  auto offset_str = "sf::Vector2f(" + x_offset + ", 0)";
  return "add_change_offset_operation(&##SPRITE_NAME##," + offset_str + "," +
         length + ");";
}

std::string code_sprite_change_y_by(const Block &block) {
  auto y_offset = block.get_bound_value("y_offset").value();
  return "##SPRITE_NAME##.move(sf::Vector2f(0.0f, " + y_offset + "));";
}

std::string code_sprite_change_y_by_in(const Block &block) {
  auto y_offset = block.get_bound_value("y_offset").value();
  auto length = block.get_bound_value("length").value();

  auto offset_str = "sf::Vector2f(0 , " + y_offset + ")";
  return "add_change_offset_operation(&##SPRITE_NAME##," + offset_str + "," +
         length + ");";
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
         " e.mouseButton.button == sf::Mouse::Left && "
         "is_mouse_over(&##SPRITE_NAME##)){\n";
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

std::string code_normalize_vector2f(const Block &block) {
  auto vector2f_name = block.get_bound_value("variable_name").value();
  return "_##SPRITE_NAME##_" + vector2f_name +
         " = normalized(_##SPRITE_NAME##_" + vector2f_name + ");";
}

std::string code_update_vector2f(const Block &block) {
  auto vector2f_name = block.get_bound_value("variable_name").value();
  auto member = block.get_bound_value("member").value();
  auto operation = block.get_bound_value("operation").value();
  auto value = block.get_bound_value("value").value();

  // vector.x += 10;
  return "_##SPRITE_NAME##_" + vector2f_name + "." + member + " " + operation +
         "= " + value + ";";
}

#endif // OUTPUT_CODE_CALL_BACKS_HPP