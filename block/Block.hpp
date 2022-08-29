#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../Globals.hpp"
#include "NODE.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
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
  const float spacing = 5, padding_left = 5, padding_right = 5, padding_up = 5,
              padding_down = 5;
  const sf::Vector2f STARTING_BLOCK_SIZE{0.0f, 45.0f};

  sf::RectangleShape block_rect;

  bool dragging = false;
  sf::Vector2f position;

public:
  Block();

  std::function<std::string(Block b)> output_code_callback;
  std::vector<NODE> childrens;

  void set_position(const sf::Vector2f pos);
  void add_node(struct NODE p_node) { childrens.push_back(p_node); }

  std::optional<std::string> get_bound_value(const std::string &query) {
    for (const auto &child : childrens) {
      if (child.bind_string == query) {
        // TODO : add a member get_value().
        // So that get_value() can return a unique values according to need.
        return child.text;
      }
    }

    auto unbound_msg_str = "[Debug] String " + query + " Possibly Unbound.";
    ERR_FAIL_COND_CRASH(false, unbound_msg_str);

    return {};
  }

  // Maybe use some dirty flag ???
  void _recalculate_rect();
  bool _process_left_click_on_children();
  void _process_events(sf::Event event);

  bool _any_node_already_pressed();
  void _deselect_all_nodes();

  void Render();
};

#endif
