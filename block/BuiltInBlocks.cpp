#include "BuiltInBlocks.hpp"
#include "../CodeGen/OutputCodeCallbacks.hpp"
#include "../Globals.hpp"

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

  // return std::move(block_program_started);
}

void block_forever(Block *block_forever) {
  block_forever->add_node(LabelNode("Forever"));
  block_forever->add_node(BlockAttachNode(""));
  block_forever->set_block_type(BLOCK_TYPES::CONTROL);
  block_forever->output_code_callback = code_sprite_forever;
  block_forever->function_identifier = "block_forever";
}

void block_if(Block *block_if) {
  block_if->add_node(LabelNode("If Condition"));
  block_if->add_node(LineInputNode("", "condition"));
  block_if->add_node(BlockAttachNode("", "", false));
  // We have an else block below.
  // The else block draws it's own rectangle.
  // So we don't need that bottom part in 'BlockAttachNode' above.
  // Otherwise there will be two rectangles in else part, which doesn't look
  // nice.
  block_if->add_node(LabelNode("Else "));
  block_if->add_node(BlockAttachNode(""));
  block_if->output_code_callback = code_sprite_if_else;
  block_if->function_identifier = "block_if";
}

void block_key_pressed(Block *block_key_pressed) {
  block_key_pressed->add_node(LabelNode("If"));
  block_key_pressed->add_node(
      DropDownNode("", "selected_key", {"W", "S", "A", "D"}));
  block_key_pressed->add_node(LabelNode("Pressed"));
  block_key_pressed->add_node(BlockAttachNode(""));
  block_key_pressed->output_code_callback = code_input_key_pressed;
  block_key_pressed->function_identifier = "block_key_pressed";
}

void block_sprite_touching(Block *block_sprite_touching) {
  block_sprite_touching->add_node(LabelNode("If touching"));
  block_sprite_touching->add_node(LineInputNode("", "target_sprite"));
  block_sprite_touching->add_node(BlockAttachNode(""));
  block_sprite_touching->output_code_callback = code_sprite_touching;
  block_sprite_touching->function_identifier = "block_sprite_touching";
}

void block_sprite_clicked(Block *block_sprite_clicked) {
  block_sprite_clicked->add_node(LabelNode("When It's Clicked"));
  block_sprite_clicked->set_block_type(BLOCK_TYPES::CONTROL);
  block_sprite_clicked->function_identifier = "block_sprite_clicked";
  block_sprite_clicked->output_code_callback = code_sprite_clicked;
}

void block_alternate_between(Block *block_alternate_between) {
  block_alternate_between->add_node(LabelNode("Alternate between this"));
  block_alternate_between->add_node(BlockAttachNode("", "", true));
  block_alternate_between->add_node(LabelNode("And This"));
  block_alternate_between->add_node(BlockAttachNode(""));
  block_alternate_between->output_code_callback = code_alternate_between;
  block_alternate_between->output_code_for_init_callback =
      code_alternate_between_init;
  block_alternate_between->function_identifier = "block_alternate_between";
}

// Motion Blocks
void block_default_character_controller(Block *block_def_char_con) {
  block_def_char_con->add_node(LabelNode("Default Character Controller:"));
  block_def_char_con->add_node(LabelNode("Speed = "));
  block_def_char_con->add_node(LineInputNode("", "speed", InputType::NUMBER));
  block_def_char_con->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_def_char_con->output_code_callback = code_default_character_controller;
  block_def_char_con->function_identifier =
      "block_default_character_controller";
}

void block_go_to_xy(Block *block_go_to_xy) {
  block_go_to_xy->add_node(LabelNode("Go to"));
  block_go_to_xy->add_node(LabelNode("X"));
  block_go_to_xy->add_node(LineInputNode("", "x", InputType::NUMBER));
  block_go_to_xy->add_node(LabelNode("Y"));
  block_go_to_xy->add_node(LineInputNode("", "y", InputType::NUMBER));
  block_go_to_xy->add_node(PickWithMouseNode(2, 4));
  block_go_to_xy->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_go_to_xy->output_code_callback = code_sprite_set_position;
  block_go_to_xy->function_identifier = "block_go_to_xy";
}

void block_move_by_vector2(Block *block_move_by_vector2) {
  block_move_by_vector2->add_node(LabelNode("Move by Vector2"));
  block_move_by_vector2->add_node(LineInputNode("", "offset"));
  block_move_by_vector2->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_move_by_vector2->output_code_callback = code_sprite_move_by_vector2;
  block_move_by_vector2->function_identifier = "block_move_by_vector2";
}

void block_glide_to_xy(Block *block_glide_to_xy) {
  block_glide_to_xy->add_node(LabelNode("Glide to"));
  block_glide_to_xy->add_node(LabelNode("X"));
  block_glide_to_xy->add_node(LineInputNode("", "x", InputType::NUMBER));
  block_glide_to_xy->add_node(LabelNode("Y"));
  block_glide_to_xy->add_node(LineInputNode("", "y", InputType::NUMBER));
  block_glide_to_xy->add_node(PickWithMouseNode(2, 4));
  block_glide_to_xy->add_node(LabelNode("For"));
  block_glide_to_xy->add_node(LineInputNode("", "length", InputType::NUMBER));
  block_glide_to_xy->add_node(LabelNode("seconds"));
  block_glide_to_xy->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_glide_to_xy->output_code_callback = code_sprite_glide_to_xy;
  block_glide_to_xy->function_identifier = "block_glide_to_xy";
}

void block_glide_point_to_point(Block *block_glide_point_to_point) {
  block_glide_point_to_point->add_node(LabelNode("Glide to"));
  block_glide_point_to_point->add_node(LabelNode("X"));
  block_glide_point_to_point->add_node(
      LineInputNode("", "x", InputType::NUMBER));
  block_glide_point_to_point->add_node(LabelNode("Y"));
  block_glide_point_to_point->add_node(
      LineInputNode("", "y", InputType::NUMBER));
  block_glide_point_to_point->add_node(PickWithMouseNode(2, 4));
  block_glide_point_to_point->add_node(LabelNode("For"));
  block_glide_point_to_point->add_node(
      LineInputNode("", "length", InputType::NUMBER));
  block_glide_point_to_point->add_node(LabelNode("seconds"));
  block_glide_point_to_point->add_node(LabelNode("To X1"));
  block_glide_point_to_point->add_node(
      LineInputNode("", "x1", InputType::NUMBER));
  block_glide_point_to_point->add_node(LabelNode("Y1"));
  block_glide_point_to_point->add_node(
      LineInputNode("", "y1", InputType::NUMBER));
  block_glide_point_to_point->add_node(PickWithMouseNode(10, 12));

  block_glide_point_to_point->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_glide_point_to_point->output_code_callback =
      code_sprite_glide_point_to_point;
  block_glide_point_to_point->function_identifier =
      "block_glide_point_to_point";
}

void block_change_x_by(Block *block_change_x_by) {
  block_change_x_by->add_node(LabelNode("Change X By"));
  block_change_x_by->add_node(LineInputNode("", "x_offset", InputType::NUMBER));
  block_change_x_by->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_x_by->output_code_callback = code_sprite_change_x_by;
  block_change_x_by->function_identifier = "block_change_x_by";
}

void block_change_x_by_in(Block *block_change_x_by_in) {
  block_change_x_by_in->add_node(LabelNode("Change X By"));
  block_change_x_by_in->add_node(
      LineInputNode("", "x_offset", InputType::NUMBER));
  block_change_x_by_in->add_node(LabelNode("For"));
  block_change_x_by_in->add_node(
      LineInputNode("", "length", InputType::NUMBER));
  block_change_x_by_in->add_node(LabelNode("seconds"));

  block_change_x_by_in->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_x_by_in->output_code_callback = code_sprite_change_x_by_in;
  block_change_x_by_in->function_identifier = "block_change_x_by_in";
}

void block_change_y_by(Block *block_change_y_by) {
  block_change_y_by->add_node(LabelNode("Change Y By"));
  block_change_y_by->add_node(LineInputNode("", "y_offset", InputType::NUMBER));
  block_change_y_by->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_y_by->output_code_callback = code_sprite_change_y_by;
  block_change_y_by->function_identifier = "block_change_y_by";
}

void block_change_y_by_in(Block *block_change_y_by_in) {
  block_change_y_by_in->add_node(LabelNode("Change Y By"));
  block_change_y_by_in->add_node(
      LineInputNode("", "y_offset", InputType::NUMBER));
  block_change_y_by_in->add_node(LabelNode("For"));
  block_change_y_by_in->add_node(
      LineInputNode("", "length", InputType::NUMBER));
  block_change_y_by_in->add_node(LabelNode("seconds"));

  block_change_y_by_in->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_MOTION;
  block_change_y_by_in->output_code_callback = code_sprite_change_y_by_in;
  block_change_y_by_in->function_identifier = "block_change_y_by_in";
}

// Draw Primitive Blocks.
void block_say(Block *block_say) {
  //   Label("Say")
  //     LineInputAttachField(message)
  // The user input is bound to ^^^^^
  // Block block_say;
  block_say->add_node(LabelNode("Say"));
  block_say->add_node(LineInputNode("", "message"));
  block_say->add_node(LabelNode("For"));
  block_say->add_node(LineInputNode("", "length", InputType::NUMBER));
  block_say->add_node(LabelNode("Seconds"));

  block_say->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_say->output_code_callback = code_sprite_say;
  block_say->function_identifier = "block_say";

  //  block_say.set_position({70.0f, 250.0f});

  // return std::move(block_say);
}

void block_draw_text(Block *block_draw_text) {
  block_draw_text->add_node(LabelNode("Draw Text : "));
  block_draw_text->add_node(LineInputNode("", "string"));
  block_draw_text->add_node(LabelNode("At X"));
  block_draw_text->add_node(LineInputNode("", "x", InputType::NUMBER));
  block_draw_text->add_node(LabelNode("Y"));
  block_draw_text->add_node(LineInputNode("", "y", InputType::NUMBER));

  block_draw_text->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_text->output_code_callback = code_draw_text;
  block_draw_text->output_code_for_init_callback = code_draw_text_init;
  block_draw_text->function_identifier = "block_draw_text";
}

void block_draw_line(Block *block_draw_line) {
  block_draw_line->add_node(LabelNode("Draw Line"));
  block_draw_line->add_node(LabelNode("From"));
  block_draw_line->add_node(LineInputNode("", "starting_pos"));
  block_draw_line->add_node(LabelNode("To"));
  block_draw_line->add_node(LineInputNode("", "ending_pos"));
  block_draw_line->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_line->output_code_callback = code_sprite_draw_line;
  block_draw_line->function_identifier = "block_draw_line";
}

void block_draw_circle(Block *block_draw_circle) {
  block_draw_circle->add_node(LabelNode("Draw Circle"));
  block_draw_circle->add_node(LabelNode("At"));
  block_draw_circle->add_node(LineInputNode("", "centre"));
  block_draw_circle->add_node(LabelNode("Radius"));
  block_draw_circle->add_node(LineInputNode("", "radius", InputType::NUMBER));
  block_draw_circle->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_circle->output_code_callback = code_sprite_draw_circle;
  block_draw_circle->function_identifier = "block_draw_circle";
}

void block_draw_rectangle(Block *block_draw_rectangle) {
  block_draw_rectangle->add_node(LabelNode("Draw Rectangle"));
  block_draw_rectangle->add_node(LabelNode("At Top Left Pos"));
  block_draw_rectangle->add_node(LineInputNode("", "top_left"));
  block_draw_rectangle->add_node(LabelNode("Size"));
  block_draw_rectangle->add_node(LineInputNode("", "rect_size"));
  block_draw_rectangle->TabItBelongsToName =
      BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_rectangle->output_code_callback = code_sprite_draw_rectangle;
  block_draw_rectangle->function_identifier = "block_draw_rectangle";
}

void block_draw_triangle(Block *block_draw_triangle) {
  // Block block_draw_triangle;
  block_draw_triangle->add_node(LabelNode("Draw Triangle"));
  block_draw_triangle->add_node(LabelNode("P1"));
  block_draw_triangle->add_node(LineInputNode("", "first_point"));
  block_draw_triangle->add_node(LabelNode("P2"));
  block_draw_triangle->add_node(LineInputNode("", "second_point"));
  block_draw_triangle->add_node(LabelNode("P3"));
  block_draw_triangle->add_node(LineInputNode("", "third_point"));
  block_draw_triangle->TabItBelongsToName =
      BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
  block_draw_triangle->output_code_callback = code_sprite_draw_triangle;
  block_draw_triangle->function_identifier = "block_draw_triangle";
}

// Create Variables Blocks.
void block_create_int(Block *block_create_int) {
  block_create_int->add_node(LabelNode("Create int"));
  block_create_int->add_node(LineInputNode("", "variable_name"));
  block_create_int->add_node(LabelNode("="));
  block_create_int->add_node(
      LineInputNode("", "variable_value", InputType::NUMBER));

  block_create_int->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_int->output_code_callback = code_create_int;
  block_create_int->function_identifier = "block_create_int";
}

void block_set_int(Block *block_set_int) {
  block_set_int->add_node(LabelNode("Set int"));
  block_set_int->add_node(LineInputNode("", "variable_name"));
  block_set_int->add_node(LabelNode("="));
  block_set_int->add_node(
      LineInputNode("", "variable_value", InputType::NUMBER));

  block_set_int->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_int->output_code_callback = code_set_int;
  block_set_int->function_identifier = "block_set_int";
}

void block_create_float(Block *block_create_float) {
  block_create_float->add_node(LabelNode("Create float"));
  block_create_float->add_node(LineInputNode("", "variable_name"));
  block_create_float->add_node(LabelNode("="));
  block_create_float->add_node(
      LineInputNode("", "variable_value", InputType::NUMBER));

  block_create_float->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_float->output_code_callback = code_create_float;
  block_create_float->function_identifier = "block_create_float";
}

void block_set_float(Block *block_set_float) {
  block_set_float->add_node(LabelNode("Set float"));
  block_set_float->add_node(LineInputNode("", "variable_name"));
  block_set_float->add_node(LabelNode("="));
  block_set_float->add_node(
      LineInputNode("", "variable_value", InputType::NUMBER));

  block_set_float->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_float->output_code_callback = code_set_float;
  block_set_float->function_identifier = "block_set_float";
}

void block_create_string(Block *block_create_string) {
  block_create_string->add_node(LabelNode("Create string"));
  block_create_string->add_node(LineInputNode("", "variable_name"));
  block_create_string->add_node(LabelNode("="));
  block_create_string->add_node(
      LineInputNode("", "variable_value", InputType::NUMBER));

  block_create_string->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_string->output_code_callback = code_create_string;
  block_create_string->function_identifier = "block_create_string";
}

void block_set_string(Block *block_set_string) {
  block_set_string->add_node(LabelNode("Set string"));
  block_set_string->add_node(LineInputNode("", "variable_name"));
  block_set_string->add_node(LabelNode("="));
  block_set_string->add_node(LineInputNode("", "variable_value"));

  block_set_string->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_string->output_code_callback = code_set_string;
  block_set_string->function_identifier = "block_set_string";
}

void block_create_vector2f(Block *block_create_vector2f) {
  block_create_vector2f->add_node(LabelNode("Create Vector2f"));
  block_create_vector2f->add_node(LineInputNode("", "variable_name"));
  block_create_vector2f->add_node(LabelNode("="));
  block_create_vector2f->add_node(LabelNode("x:"));
  block_create_vector2f->add_node(
      LineInputNode("", "variable_value_x", InputType::NUMBER));
  block_create_vector2f->add_node(LabelNode("y:"));
  block_create_vector2f->add_node(
      LineInputNode("", "variable_value_y", InputType::NUMBER));

  block_create_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_create_vector2f->output_code_callback = code_create_vector2f;
  block_create_vector2f->function_identifier = "block_create_vector2f";
}

void block_set_vector2f(Block *block_set_vector2f) {
  block_set_vector2f->add_node(LabelNode("Set Vector2f"));
  block_set_vector2f->add_node(LineInputNode("", "variable_name"));
  block_set_vector2f->add_node(LabelNode("="));
  block_set_vector2f->add_node(LabelNode("x:"));
  block_set_vector2f->add_node(
      LineInputNode("", "variable_value_x", InputType::NUMBER));
  block_set_vector2f->add_node(LabelNode("y:"));
  block_set_vector2f->add_node(
      LineInputNode("", "variable_value_y", InputType::NUMBER));

  block_set_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_set_vector2f->output_code_callback = code_set_vector2f;
  block_set_vector2f->function_identifier = "block_set_vector2f";
}

void block_normalize_vector2f(Block *block_normalize_vector2f) {
  block_normalize_vector2f->add_node(LabelNode("Normalize Vector2f"));
  block_normalize_vector2f->add_node(LineInputNode("", "variable_name"));
  block_normalize_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_normalize_vector2f->output_code_callback = code_normalize_vector2f;
  block_normalize_vector2f->function_identifier = "block_normalize_vector2f";
}

void block_update_vector2f(Block *block_update_vector2f) {
  block_update_vector2f->add_node(LabelNode("Update Vector2f"));
  block_update_vector2f->add_node(LineInputNode("", "variable_name"));
  block_update_vector2f->add_node(LabelNode("."));
  block_update_vector2f->add_node(DropDownNode("", "member", {"x", "y"}));
  block_update_vector2f->add_node(
      DropDownNode("", "operation", {"+", "-", "*", "/"}));
  block_update_vector2f->add_node(LabelNode("="));
  block_update_vector2f->add_node(LineInputNode("", "value"));
  block_update_vector2f->TabItBelongsToName = BLOCKS_TAB_NAME::TAB_VARIABLES;
  block_update_vector2f->output_code_callback = code_update_vector2f;
  block_update_vector2f->function_identifier = "block_update_vector2f";
}

} // namespace BUILT_IN_BLOCKS