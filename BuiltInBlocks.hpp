#ifndef BUILT_IN_BLOCKS_HPP
#define BUILT_IN_BLOCKS_HPP

#include "Globals.hpp"
#include "code_gen.hpp"

namespace BUILT_IN_BLOCKS {

// Control Blocks
void block_program_started(Block *block_program_started) {
  // Block block_program_started;
  block_program_started->add_node(LabelNode("When Program Starts"));
  block_program_started->set_block_type(BLOCK_TYPES::CONTROL);
  block_program_started->function_identifier = "block_program_started";

  // Editor blocks position is set by the tab view anyway.
  // Newly spawned blocks spawn at mouse position.
  // block_program_started.set_position({425.0f, 25.0f});
  block_program_started->_recalculate_rect();
  // return std::move(block_program_started);
}

void block_forever(Block *block_forever) {
  // Block block_forever;
  block_forever->add_node(LabelNode("Forever"));
  block_forever->add_node(BlockAttachNode(""));
  block_forever->set_block_type(BLOCK_TYPES::CONTROL);
  block_forever->output_code_callback = code_sprite_forever;
  block_forever->function_identifier = "block_forever";

  // block_forever.set_position({});
  block_forever->_recalculate_rect();
  // return std::move(block_forever);
}

void block_if(Block *block_if) {
  // Block block_if;
  block_if->add_node(LabelNode("If Condition"));
  block_if->add_node(LineInputAttachFieldNode("", "condition"));
  block_if->add_node(BlockAttachNode(""));
  block_if->add_node(LabelNode("Else "));
  block_if->add_node(BlockAttachNode(""));
  block_if->output_code_callback = code_sprite_if_else;
  block_if->function_identifier = "block_if";

  // block_if.set_position({});
  block_if->_recalculate_rect();
  // return std::move(block_if);
}

void block_key_pressed(Block *block_key_pressed) {
  // Block block_key_pressed;
  block_key_pressed->add_node(LabelNode("If"));
  block_key_pressed->add_node(DropDownNode("", "selected_key"));
  block_key_pressed->add_node(LabelNode("Pressed"));
  block_key_pressed->add_node(BlockAttachNode(""));
  block_key_pressed->output_code_callback = code_input_key_pressed;
  block_key_pressed->function_identifier = "block_key_pressed";

  // block_key_pressed.set_position({640.0f, 560.0f});
  block_key_pressed->_recalculate_rect();
  // return std::move(block_key_pressed);
}

void block_sprite_touching(Block *block_sprite_touching) {
  // Block block_sprite_touching;
  block_sprite_touching->add_node(LabelNode("If touching"));
  block_sprite_touching->add_node(
      LineInputAttachFieldNode("", "target_sprite"));
  block_sprite_touching->add_node(BlockAttachNode(""));
  block_sprite_touching->output_code_callback = code_sprite_touching;
  block_sprite_touching->function_identifier = "block_sprite_touching";

  // block_sprite_touching.set_position({640.0f, 560.0f});
  block_sprite_touching->_recalculate_rect();
  // return std::move(block_sprite_touching);
}

void block_sprite_clicked(Block *block_sprite_clicked) {
  // Block block_sprite_clicked;
  block_sprite_clicked->add_node(LabelNode("When It's Clicked"));
  block_sprite_clicked->set_block_type(BLOCK_TYPES::CONTROL);
  block_sprite_clicked->function_identifier = "block_sprite_clicked";
  block_sprite_clicked->output_code_callback = code_sprite_clicked;

  // block_sprite_clicked.set_position({425.0f, 25.0f});
  block_sprite_clicked->_recalculate_rect();
  // return std::move(block_sprite_clicked);
}

// Motion Blocks
void block_go_to_xy(Block *block_go_to_xy) {
  // Block block_go_to_xy;
  block_go_to_xy->add_node(LabelNode("Go to"));
  block_go_to_xy->add_node(LabelNode("X"));
  block_go_to_xy->add_node(LineInputAttachFieldNode("", "x"));
  block_go_to_xy->add_node(LabelNode("Y"));
  block_go_to_xy->add_node(LineInputAttachFieldNode("", "y"));
  block_go_to_xy->add_node(ButtonNode("Pick^"));
  block_go_to_xy->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_go_to_xy->output_code_callback = code_sprite_set_position;
  block_go_to_xy->function_identifier = "block_go_to_xy";

  // block_go_to_xy.set_position({70.0f, 600.0f});
  block_go_to_xy->_recalculate_rect();
  // return std::move(block_go_to_xy);
}

void block_glide_to_xy(Block *block_glide_to_xy) {
  // Block block_glide_to_xy;
  block_glide_to_xy->add_node(LabelNode("Glide to"));
  block_glide_to_xy->add_node(LabelNode("X"));
  block_glide_to_xy->add_node(LineInputAttachFieldNode("", "x"));
  block_glide_to_xy->add_node(LabelNode("Y"));
  block_glide_to_xy->add_node(LineInputAttachFieldNode("", "y"));
  block_glide_to_xy->add_node(ButtonNode("Pick^"));
  block_glide_to_xy->add_node(LabelNode("For"));
  block_glide_to_xy->add_node(LineInputAttachFieldNode("", "length"));
  block_glide_to_xy->add_node(LabelNode("seconds"));
  block_glide_to_xy->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_glide_to_xy->output_code_callback = code_sprite_glide_to_xy;
  block_glide_to_xy->function_identifier = "block_glide_to_xy";

  // block_glide_to_xy.set_position({70.0f, 600.0f});
  block_glide_to_xy->_recalculate_rect();
  // return std::move(block_glide_to_xy);
}

void block_glide_point_to_point(Block *block_glide_point_to_point) {
  // Block block_glide_point_to_point;
  block_glide_point_to_point->add_node(LabelNode("Glide to"));
  block_glide_point_to_point->add_node(LabelNode("X"));
  block_glide_point_to_point->add_node(LineInputAttachFieldNode("", "x"));
  block_glide_point_to_point->add_node(LabelNode("Y"));
  block_glide_point_to_point->add_node(LineInputAttachFieldNode("", "y"));
  block_glide_point_to_point->add_node(ButtonNode("Pick^"));
  block_glide_point_to_point->add_node(LabelNode("For"));
  block_glide_point_to_point->add_node(LineInputAttachFieldNode("", "length"));
  block_glide_point_to_point->add_node(LabelNode("seconds"));
  block_glide_point_to_point->add_node(LabelNode("To X1"));
  block_glide_point_to_point->add_node(LineInputAttachFieldNode("", "x1"));
  block_glide_point_to_point->add_node(LabelNode("Y1"));
  block_glide_point_to_point->add_node(LineInputAttachFieldNode("", "y1"));

  block_glide_point_to_point->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_glide_point_to_point->output_code_callback =
      code_sprite_glide_point_to_point;
  block_glide_point_to_point->function_identifier =
      "block_glide_point_to_point";

  // block_glide_point_to_point.set_position({70.0f, 600.0f});
  block_glide_point_to_point->_recalculate_rect();
  // return std::move(block_glide_point_to_point);
}

void block_change_x_by(Block *block_change_x_by) {
  // Block block_change_x_by;
  block_change_x_by->add_node(LabelNode("Change X By"));
  block_change_x_by->add_node(LineInputAttachFieldNode("", "x_offset"));
  block_change_x_by->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_x_by->output_code_callback = code_sprite_change_x_by;
  block_change_x_by->function_identifier = "block_change_x_by";

  // block_change_x_by.set_position({70.0f, 400.0f});
  block_change_x_by->_recalculate_rect();
  // return std::move(block_change_x_by);
}

void block_change_y_by(Block *block_change_y_by) {
  // Block block_change_y_by;
  block_change_y_by->add_node(LabelNode("Change Y By"));
  block_change_y_by->add_node(LineInputAttachFieldNode("", "y_offset"));
  block_change_y_by->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_y_by->output_code_callback = code_sprite_change_y_by;
  block_change_y_by->function_identifier = "block_change_y_by";

  // block_change_y_by.set_position({70.0f, 480.0f});
  block_change_y_by->_recalculate_rect();
  // return std::move(block_change_y_by);
}

// Draw Primitive Blocks.
void block_say(Block *block_say) {
  //   Label("Say")
  //     LineInputAttachField(message)
  // The user input is bound to ^^^^^
  // Block block_say;
  block_say->add_node(LabelNode("Say"));
  block_say->add_node(LineInputAttachFieldNode("", "message"));
  block_say->add_node(LabelNode("For"));
  block_say->add_node(LineInputAttachFieldNode("", "length"));
  block_say->add_node(LabelNode("Seconds"));

  block_say->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_say->output_code_callback = code_sprite_say;
  block_say->function_identifier = "block_say";

  //  block_say.set_position({70.0f, 250.0f});
  block_say->_recalculate_rect();
  // return std::move(block_say);
}

void block_draw_line(Block *block_draw_line) {
  // Block block_draw_line;
  block_draw_line->add_node(LabelNode("Draw Line"));
  block_draw_line->add_node(LabelNode("From"));
  block_draw_line->add_node(LineInputAttachFieldNode("", "starting_pos"));
  block_draw_line->add_node(LabelNode("To"));
  block_draw_line->add_node(LineInputAttachFieldNode("", "ending_pos"));
  block_draw_line->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_line->output_code_callback = code_sprite_draw_line;
  block_draw_line->function_identifier = "block_draw_line";

  // block_draw_line.set_position({430.0f, 310.0f});
  block_draw_line->_recalculate_rect();
  // return std::move(block_draw_line);
}

void block_draw_circle(Block *block_draw_circle) {
  // Block block_draw_circle;
  block_draw_circle->add_node(LabelNode("Draw Circle"));
  block_draw_circle->add_node(LabelNode("At"));
  block_draw_circle->add_node(LineInputAttachFieldNode("", "centre"));
  block_draw_circle->add_node(LabelNode("Radius"));
  block_draw_circle->add_node(LineInputAttachFieldNode("", "radius"));
  block_draw_circle->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_circle->output_code_callback = code_sprite_draw_circle;
  block_draw_circle->function_identifier = "block_draw_circle";

  // block_draw_circle.set_position({430.0f, 370.0f});
  block_draw_circle->_recalculate_rect();
  // return std::move(block_draw_circle);
}

void block_draw_rectangle(Block *block_draw_rectangle) {
  // Block block_draw_rectangle;
  block_draw_rectangle->add_node(LabelNode("Draw Rectangle"));
  block_draw_rectangle->add_node(LabelNode("At Top Left Pos"));
  block_draw_rectangle->add_node(LineInputAttachFieldNode("", "top_left"));
  block_draw_rectangle->add_node(LabelNode("Size"));
  block_draw_rectangle->add_node(LineInputAttachFieldNode("", "rect_size"));
  block_draw_rectangle->TabItBelongsToName =
      BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_rectangle->output_code_callback = code_sprite_draw_rectangle;
  block_draw_rectangle->function_identifier = "block_draw_rectangle";

  // block_draw_rectangle.set_position({430.0f, 435.0f});
  block_draw_rectangle->_recalculate_rect();
  // return std::move(block_draw_rectangle);
}

void block_draw_triangle(Block *block_draw_triangle) {
  // Block block_draw_triangle;
  block_draw_triangle->add_node(LabelNode("Draw Triangle"));
  block_draw_triangle->add_node(LabelNode("P1"));
  block_draw_triangle->add_node(LineInputAttachFieldNode("", "first_point"));
  block_draw_triangle->add_node(LabelNode("P2"));
  block_draw_triangle->add_node(LineInputAttachFieldNode("", "second_point"));
  block_draw_triangle->add_node(LabelNode("P3"));
  block_draw_triangle->add_node(LineInputAttachFieldNode("", "third_point"));
  block_draw_triangle->TabItBelongsToName =
      BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_triangle->output_code_callback = code_sprite_draw_triangle;
  block_draw_triangle->function_identifier = "block_draw_triangle";

  // block_draw_triangle.set_position({430.0f, 490.0f});
  block_draw_triangle->_recalculate_rect();
  // return std::move(block_draw_triangle);
}

// Create Variables Blocks.
void block_create_int(Block *block_create_int) {
  block_create_int->add_node(LabelNode("Create int"));
  block_create_int->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_create_int->add_node(LabelNode("="));
  block_create_int->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_create_int->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_int->output_code_callback = code_create_int;
  block_create_int->function_identifier = "block_create_int";

  block_create_int->_recalculate_rect();
}

void block_set_int(Block *block_set_int) {
  block_set_int->add_node(LabelNode("Set int"));
  block_set_int->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_set_int->add_node(LabelNode("="));
  block_set_int->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_set_int->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_int->output_code_callback = code_set_int;
  block_set_int->function_identifier = "block_set_int";

  block_set_int->_recalculate_rect();
}

void block_create_float(Block *block_create_float) {
  block_create_float->add_node(LabelNode("Create float"));
  block_create_float->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_create_float->add_node(LabelNode("="));
  block_create_float->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_create_float->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_float->output_code_callback = code_create_float;
  block_create_float->function_identifier = "block_create_float";

  block_create_float->_recalculate_rect();
}

void block_set_float(Block *block_set_float) {
  block_set_float->add_node(LabelNode("Set float"));
  block_set_float->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_set_float->add_node(LabelNode("="));
  block_set_float->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_set_float->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_float->output_code_callback = code_set_float;
  block_set_float->function_identifier = "block_set_float";

  block_set_float->_recalculate_rect();
}

void block_create_string(Block *block_create_string) {
  block_create_string->add_node(LabelNode("Create string"));
  block_create_string->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_create_string->add_node(LabelNode("="));
  block_create_string->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_create_string->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_string->output_code_callback = code_create_string;
  block_create_string->function_identifier = "block_create_string";

  block_create_string->_recalculate_rect();
}

void block_set_string(Block *block_set_string) {
  block_set_string->add_node(LabelNode("Set string"));
  block_set_string->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_set_string->add_node(LabelNode("="));
  block_set_string->add_node(LineInputAttachFieldNode("", "variable_value"));

  block_set_string->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_string->output_code_callback = code_set_string;
  block_set_string->function_identifier = "block_set_string";

  block_set_string->_recalculate_rect();
}

void block_create_vector2f(Block *block_create_vector2f) {
  block_create_vector2f->add_node(LabelNode("Create Vector2f"));
  block_create_vector2f->add_node(
      LineInputAttachFieldNode("", "variable_name"));
  block_create_vector2f->add_node(LabelNode("="));
  block_create_vector2f->add_node(LabelNode("x:"));
  block_create_vector2f->add_node(
      LineInputAttachFieldNode("", "variable_value_x"));
  block_create_vector2f->add_node(LabelNode("y:"));
  block_create_vector2f->add_node(
      LineInputAttachFieldNode("", "variable_value_y"));

  block_create_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_vector2f->output_code_callback = code_create_vector2f;
  block_create_vector2f->function_identifier = "block_create_vector2f";

  block_create_vector2f->_recalculate_rect();
}

void block_set_vector2f(Block *block_set_vector2f) {
  block_set_vector2f->add_node(LabelNode("Set Vector2f"));
  block_set_vector2f->add_node(LineInputAttachFieldNode("", "variable_name"));
  block_set_vector2f->add_node(LabelNode("="));
  block_set_vector2f->add_node(LabelNode("x:"));
  block_set_vector2f->add_node(
      LineInputAttachFieldNode("", "variable_value_x"));
  block_set_vector2f->add_node(LabelNode("y:"));
  block_set_vector2f->add_node(
      LineInputAttachFieldNode("", "variable_value_y"));

  block_set_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_vector2f->output_code_callback = code_set_vector2f;
  block_set_vector2f->function_identifier = "block_set_vector2f";

  block_set_vector2f->_recalculate_rect();
}

} // namespace BUILT_IN_BLOCKS

#endif
