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
  std::vector<NODE> childrens;

public:
  sf::Vector2f position;

  void add_node(struct NODE p_node) { childrens.push_back(p_node); }

  sf::Vector2f line_input_field_rect_size() { return {25, 35}; }
  void draw_line_input_attach_field(const sf::Vector2f p_position);

  void _process_inputs(sf::Event event);
  void Render();
};

#endif
