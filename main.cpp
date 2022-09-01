#include "Globals.hpp"
#include "block/Block.hpp"
#include "block/LineInput.hpp"
#include "block/NODEBaseClass.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SHOW_FPS
#define SHOW_MOUSE_POS

int height = 800;
int width = 800;

sf::Vector2i mouse_position;

// Externs from globals.hpp
sf::Font font;
sf::Text draw_text_label;
sf::RenderWindow window;

#ifdef SHOW_FPS
float get_fps() {
  static sf::Clock clock;
  static sf::Time last_time = clock.getElapsedTime();
  sf::Time current_time = clock.getElapsedTime();
  float fps = 1.f / (current_time.asSeconds() - last_time.asSeconds());
  last_time = current_time;

  return fps;
}
#endif

std::string code_sprite_set_position(Block block) {
  //"x" -> block.childrens.at(2).text
  //"y" -> block.childrens.at(4).text
  auto x_pos = block.get_bound_value("x").value();
  auto y_pos = block.get_bound_value("y").value();
  return "sprite.setPosition(" + x_pos + ", " + y_pos + ");";
}

std::string code_sprite_if(Block block) { return "if(some_condition)"; }

std::string code_sprite_say(Block block) {
  auto message = block.get_bound_value("message").value();
  return "std::cout << \"" + message + "\" << \"\\n\";";
}

std::string code_sprite_change_x_by(Block block) {
  auto x_offset = block.get_bound_value("x_offset").value();
  return "sprite.move(sf::Vector2f(" + x_offset + ", 0.0f));";
}

std::string code_sprite_change_y_by(Block block) {
  auto y_offset = block.get_bound_value("y_offset").value();
  return "sprite.move(sf::Vector2f(0.0f, " + y_offset + "));";
}

int main() {

  init_global_font_and_label();

  height = sf::VideoMode::getDesktopMode().height;
  width = sf::VideoMode::getDesktopMode().width;

  window.create(sf::VideoMode(width, height), "SClone V2");

#ifdef SHOW_FPS
  sf::Text show_fps_btn;
  show_fps_btn.setString("FPS:");
  show_fps_btn.setFont(font);
  show_fps_btn.setPosition(sf::Vector2f(5, 40));
  show_fps_btn.setFillColor(sf::Color::Black);
#endif

#ifdef SHOW_MOUSE_POS
  sf::Text show_mouse_pos_text;
  show_mouse_pos_text.setString("");
  show_mouse_pos_text.setFont(font);
  show_mouse_pos_text.setPosition(sf::Vector2f(5, 5));
  show_mouse_pos_text.setFillColor(sf::Color::Black);
#endif

  LineInput sprite_name;
  sprite_name.position = {250.0f, 35.0f};
  sprite_name.input_text = "cat";
  sprite_name.line_input_active = false;

  std::vector<Block> blocks;

  Block block_program_started;
  block_program_started.add_node(LabelNode("When Program Starts"));
  block_program_started.set_block_type(BLOCK_TYPES::CONTROL);

  block_program_started.set_position({425.0f, 25.0f});
  block_program_started._recalculate_rect();

  Block block_if;
  block_if.add_node(LabelNode("If (some condition)"));
  block_if.add_node(BlockAttachNode(""));
  block_if.output_code_callback = code_sprite_if;

  block_if.set_position({780.0f, 25.0f});
  block_if._recalculate_rect();

  //   Label("Say")
  //     LineInputAttachField(message)
  // The user input is bound to ^^^^^
  Block block_say;
  block_say.add_node(LabelNode("Say"));
  block_say.add_node(LineInputAttachFieldNode("", "message"));
  block_say.output_code_callback = code_sprite_say;

  block_say.set_position({200.0f, 250.0f});
  block_say._recalculate_rect();

  Block block_go_to_xy;
  block_go_to_xy.add_node(LabelNode("Go to"));
  block_go_to_xy.add_node(LabelNode("X"));
  block_go_to_xy.add_node(LineInputAttachFieldNode("", "x"));
  block_go_to_xy.add_node(LabelNode("Y"));
  block_go_to_xy.add_node(LineInputAttachFieldNode("", "y"));
  block_go_to_xy.add_node(ButtonNode("Pick^"));
  block_go_to_xy.output_code_callback = code_sprite_set_position;

  block_go_to_xy.set_position({200.0f, 600.0f});
  block_go_to_xy._recalculate_rect();

  Block block_change_x_by;
  block_change_x_by.add_node(LabelNode("Change X By"));
  block_change_x_by.add_node(LineInputAttachFieldNode("", "x_offset"));
  block_change_x_by.output_code_callback = code_sprite_change_x_by;

  block_change_x_by.set_position({200.0f, 400.0f});
  block_change_x_by._recalculate_rect();

  Block block_change_y_by;
  block_change_y_by.add_node(LabelNode("Change Y By"));
  block_change_y_by.add_node(LineInputAttachFieldNode("", "y_offset"));
  block_change_y_by.output_code_callback = code_sprite_change_y_by;

  block_change_y_by.set_position({200.0f, 480.0f});
  block_change_y_by._recalculate_rect();

  blocks.push_back(block_program_started);
  blocks.push_back(block_if);
  blocks.push_back(block_say);
  blocks.push_back(block_go_to_xy);
  blocks.push_back(block_change_x_by);
  blocks.push_back(block_change_y_by);

  blocks.at(2).attach_block_next(&blocks.at(3));

  while (window.isOpen()) {
    bool middle_click = false;

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyReleased &&
           event.key.code == sf::Keyboard::Escape)) {
        window.close();
      } else if (event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::LAlt) {
        // To Generate code.
        std::cout << "\n\n[Generated Code]\n";
        for (auto block : blocks) {
          if (!block.is_control_block()) {
            continue;
          }

          auto output_code = block.get_code();

          std::cout << output_code << "\n";
        }
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Middle) {
        middle_click = true;
      }

      sprite_name.handle_inputs(event);

      for (auto &block : blocks) {
        block._process_events(event);
      }
    }

    mouse_position = sf::Mouse::getPosition(window);

    window.clear(sf::Color(0, 255, 204));

    Block *current_dragging_block_ref = nullptr;
    for (auto &block : blocks) {
      if (block.dragging) {
        current_dragging_block_ref = &block;
        break;
      }
    }

    auto get_block_it_is_attached_to = [&blocks](Block *block_to_test) {
      Block *parent = nullptr;

      for (auto &block : blocks) {
        if (&block.next_block == nullptr) {
          continue;
        }

        if (block.next_block == block_to_test) {
          parent = &block;
          break;
        }
      }
      return parent;
    };

    bool is_any_block_being_dragged = current_dragging_block_ref != nullptr;
    if (is_any_block_being_dragged) {
      for (auto &block : blocks) {
        if (current_dragging_block_ref == &block) {
          continue;
        }

        bool attach_block_requested = middle_click;

        if (block.can_mouse_snap_to_top()) {
          if (attach_block_requested) {
            Block *parent = get_block_it_is_attached_to(&block);
            if (parent != nullptr) {
              // It is already attached to some block.
              // So CurrentlyDraggedBlock will be attached to that parent.
              parent->attach_block_next(current_dragging_block_ref);
            }
            // And that parent's previously attached block('block') will be
            // attached to currently dragged block. Meaning we insert the
            // currently dragged block between them two.
            current_dragging_block_ref->dragging = false;
            current_dragging_block_ref->attach_block_next(&block);
            continue;
          } else {
            block.show_previous_block_snap_hint();
          }
        }

        if (block.can_mouse_snap_to_bottom()) {
          if (attach_block_requested) {
            current_dragging_block_ref->dragging = false;
            block.attach_block_next(current_dragging_block_ref);
            continue;
          } else {
            block.show_next_block_snap_hint();
          }
        }

        if (block.can_mouse_snap_to_inside()) {
          if (attach_block_requested) {
            current_dragging_block_ref->dragging = false;
            block.attach_block_inside(current_dragging_block_ref);
          } else {
            block.show_inside_block_snap_hint();
          }
        }
      }
    }
    for (auto block : blocks) {
      block.Render();
    }

#ifdef SHOW_FPS
    show_fps_btn.setString("FPS: " + std::to_string(int(get_fps())));
    window.draw(show_fps_btn);
#endif

#ifdef SHOW_MOUSE_POS
    show_mouse_pos_text.setString("X: " + std::to_string(mouse_position.x) +
                                  " Y: " + std::to_string(mouse_position.y));
    window.draw(show_mouse_pos_text);
#endif

    // sprite_name.Render();

    window.display();
  }

  return 0;
}
