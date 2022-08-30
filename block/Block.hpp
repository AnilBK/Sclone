#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Globals.hpp"
#include "NODEBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <optional>

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
  const float spacing = 10, padding_left = 5, padding_right = 5, padding_up = 5,
              padding_down = 5;
  const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

  sf::RectangleShape block_rect;

  bool dragging = false;
  sf::Vector2f position;

public:
  Block();

  std::function<std::string(Block b)> output_code_callback;
  std::vector<std::shared_ptr<NODEBaseClass>> childrens;

  void set_position(const sf::Vector2f pos);

  template <class T> void add_node(T node_class) {
    auto u_block = std::make_shared<T>(node_class);
    childrens.push_back(std::move(u_block));
  }

  std::optional<std::string> get_bound_value(const std::string &query);

  // Maybe use some dirty flag ???
  void _recalculate_rect();
  bool _process_left_click_on_children(sf::Event event);
  void _process_events(sf::Event event);

  bool _any_node_already_pressed();
  void _deselect_all_nodes();

  void Render();
};

#endif
