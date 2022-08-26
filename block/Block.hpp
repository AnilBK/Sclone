#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Globals.hpp"
#include <SFML/Graphics.hpp>

// Final Block Output:
// VVVVVVVVVVVVVVVVVVVVVVVVVV
// Say "Text" for "2" seconds.
// The Generator API.
//   Label("Say")
//     LineInputAttachField(text_variable)
//       Label("for")
//         LineInputAttachField(time_variable)
//           Label("seconds").

// With this new API.
// Print "String" becomes
//   Label("Print")
//     LineInputAttachField(Str)

class Block {

private:
  const float spacing = 5, padding_left = 5, padding_right = 5, padding_up = 5,
              padding_down = 5;
  const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

  sf::RectangleShape block_rect;

  bool dragging = false;

  std::vector<NODE> childrens;

  float get_node_size_x(const NODE *node);

public:
  sf::Vector2f position;

  Block();
  void add_node(struct NODE p_node) { childrens.push_back(p_node); }

  sf::Vector2f line_input_field_rect_size() { return {25, 35}; }
  void draw_line_input_attach_field(const sf::Vector2f p_position);

  // Maybe use some dirty flag ???
  void _recalculate_rect();
  void _process_events(sf::Event event);

  void Render();
};

#endif
