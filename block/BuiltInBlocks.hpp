#ifndef BUILT_IN_BLOCKS_HPP
#define BUILT_IN_BLOCKS_HPP

#include "../block/Block.hpp"

namespace BUILT_IN_BLOCKS {
// These functions take a pointer to a 'block' and add all the required
// childrens. The 'block' passed to it is supposed to have no childrens at all.

//------------------------------------------------------------------
// Final Block Output:
// .-----------------------------.
// | Say "Text" for "2" seconds. |
// .-----------------------------.

// Stacking of children to form the above output:
//   Label("Say")
//     LineInputAttachField(text_variable)
//       Label("for")
//         LineInputAttachField(time_variable)
//           Label("seconds").
//------------------------------------------------------------------

// Control Blocks
void block_program_started(Block *block_program_started);
void block_forever(Block *block_forever);
void block_if(Block *block_if);
void block_key_pressed(Block *block_key_pressed);
void block_sprite_touching(Block *block_sprite_touching);
void block_sprite_clicked(Block *block_sprite_clicked);
void block_alternate_between(Block *block_alternate_between);

// Motion Blocks
void block_default_character_controller(Block *block_def_char_con);
void block_go_to_xy(Block *block_go_to_xy);
void block_glide_to_xy(Block *block_glide_to_xy);
void block_glide_point_to_point(Block *block_glide_point_to_point);
void block_change_x_by(Block *block_change_x_by);
void block_change_y_by(Block *block_change_y_by);

// Draw Primitive Blocks.
void block_say(Block *block_say);
void block_draw_text(Block *block_draw_text);
void block_draw_line(Block *block_draw_line);
void block_draw_circle(Block *block_draw_circle);
void block_draw_rectangle(Block *block_draw_rectangle);
void block_draw_triangle(Block *block_draw_triangle);

// Create Variables Blocks.
void block_create_int(Block *block_create_int);
void block_set_int(Block *block_set_int);
void block_create_float(Block *block_create_float);
void block_set_float(Block *block_set_float);
void block_create_string(Block *block_create_string);
void block_set_string(Block *block_set_string);
void block_create_vector2f(Block *block_create_vector2f);
void block_set_vector2f(Block *block_set_vector2f);

} // namespace BUILT_IN_BLOCKS

#endif
