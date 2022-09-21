#include "BuiltInBlocks.hpp"
#include "Globals.hpp"
#include "block/Block.hpp"
#include "block/Button.hpp"
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

// A bounded function returns a 'Block'.
using Block_fn = std::function<Block()>;
std::vector<std::pair<std::string, Block_fn>> bound_blocks;

// Bound a function pointer to the given string.
void BIND_BLOCK(std::string bind_string, Block_fn fn_that_returns_the_block) {
  for (auto b_block : bound_blocks) {
    auto b_bind_string = b_block.first;
    if (b_bind_string == bind_string) {
      auto already_bound_msg_str = "[Debug] String \"" + bind_string +
                                   "\" Previously bound to some function.";
      ERR_FAIL_COND_CRASH(false, already_bound_msg_str);
    }
  }

  bound_blocks.push_back({bind_string, fn_that_returns_the_block});
}

// A 'Block_fn' returns the block.
// The returned block contains the function identifer string which identifies
// that block. Bind that return string to the function that generated the block
// in the first place.
void BIND_BLOCK(Block_fn fn_that_returns_the_block) {
  auto bind_string = fn_that_returns_the_block().function_identifier;
  BIND_BLOCK(bind_string, fn_that_returns_the_block);
}

// Return the function pointer bound with the given string.
std::optional<Block_fn> GET_BOUND_BLOCK_FN(const std::string &query) {
  for (const auto &block : bound_blocks) {
    auto name = block.first;
    if (name == query) {
      return block.second;
    }
  }

  auto unbound_msg_str = "[Debug] Function \"" + query + "\" Possibly Unbound.";
  ERR_FAIL_COND_CRASH(false, unbound_msg_str);

  return {};
}

int main() {
  init_global_font_and_label();

  sf::Font alaska;
  ERR_FAIL_COND_CRASH(alaska.loadFromFile("alaska.ttf"),
                      "Error Loading Font \"alaska\".");

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

  // All these bindings because a 'Block' has shared_ptr stuffs.
  // Duplicating a block just messes everything.
  // I guess will be fine in the long run.

  // IMPORTANT: 'Bind String' should be same as 'Function Identifiers' contained
  // in the 'block' returned by that function. We use this identifiers to query
  // the function at runtime. Maybe move the identifiers to 'Built in
  // blocks.hpp'.
  // This problem is solved by BIND_BLOCK which gets the bind string itself.

  /*
    BIND_BLOCK("block_say", BUILT_IN_BLOCKS::block_say);
    BIND_BLOCK("block_program_started", BUILT_IN_BLOCKS::block_program_started);
    BIND_BLOCK("block_forever", BUILT_IN_BLOCKS::block_forever);
    BIND_BLOCK("block_if", BUILT_IN_BLOCKS::block_if);
    BIND_BLOCK("block_key_pressed", BUILT_IN_BLOCKS::block_key_pressed);
    BIND_BLOCK("block_s_pressed", BUILT_IN_BLOCKS::block_s_pressed);

    BIND_BLOCK("block_go_to_xy", BUILT_IN_BLOCKS::block_go_to_xy);
    BIND_BLOCK("block_change_x_by", BUILT_IN_BLOCKS::block_change_x_by);
    BIND_BLOCK("block_change_y_by", BUILT_IN_BLOCKS::block_change_y_by);

    BIND_BLOCK("block_draw_line", BUILT_IN_BLOCKS::block_draw_line);
    BIND_BLOCK("block_draw_circle", BUILT_IN_BLOCKS::block_draw_circle);
    BIND_BLOCK("block_draw_rectangle", BUILT_IN_BLOCKS::block_draw_rectangle);
    BIND_BLOCK("block_draw_triangle", BUILT_IN_BLOCKS::block_draw_triangle);
  */

  // Same as Above.
  // But We don't need to manually place strings in this case.
  // The only difference being it fetches the bind string at run time unless
  // optimized by the compiler.
  // The only disadvantage is it calls the function to get the bind string and
  // binds it.

  BIND_BLOCK(BUILT_IN_BLOCKS::block_say);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_program_started);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_forever);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_if);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_key_pressed);

  BIND_BLOCK(BUILT_IN_BLOCKS::block_go_to_xy);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_change_x_by);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_change_y_by);

  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_line);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_circle);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_rectangle);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_triangle);

  /*
    // Case 1:
    //  Get the function pointer and call the function which returns the block.
    Block block_say = GET_BOUND_BLOCK_FN("block_say").value()();
    // Case 2:
    //  These are editor blocks, that means we can assign the functions at
    // compile time. No need to iterate through the entire vector comparing the
    // strings.
    Block block_say = BUILT_IN_BLOCKS::block_say();

    // Both are valid cases.
    // Case 1 is definitely suited for run time function quering.
  */

  Block block_say = BUILT_IN_BLOCKS::block_say();
  Block block_program_started = BUILT_IN_BLOCKS::block_program_started();
  Block block_forever = BUILT_IN_BLOCKS::block_forever();
  Block block_if = BUILT_IN_BLOCKS::block_if();
  Block block_key_pressed = BUILT_IN_BLOCKS::block_key_pressed();

  Block block_go_to_xy = BUILT_IN_BLOCKS::block_go_to_xy();
  Block block_change_x_by = BUILT_IN_BLOCKS::block_change_x_by();
  Block block_change_y_by = BUILT_IN_BLOCKS::block_change_y_by();

  Block block_draw_line = BUILT_IN_BLOCKS::block_draw_line();
  Block block_draw_circle = BUILT_IN_BLOCKS::block_draw_circle();
  Block block_draw_rectangle = BUILT_IN_BLOCKS::block_draw_rectangle();
  Block block_draw_triangle = BUILT_IN_BLOCKS::block_draw_triangle();

  sf::Font alaska;
  ERR_FAIL_COND_CRASH(alaska.loadFromFile("alaska.ttf"),
                      "Error Loading Font \"alaska\".");

  BLOCKS_TAB_NAME currently_selected_tab = BLOCKS_TAB_NAME::TAB_CONTROL;

  sf::RectangleShape blocks_tab;
  blocks_tab.setPosition(800, 85);
  blocks_tab.setSize({width - 800, height - 100});
  blocks_tab.setFillColor(sf::Color(204, 204, 204));

  // Buttons can have individual colors.
  // But let's not give them.
  // Since they are a part of a tab bar, every buttons should have a same color.
  Button BTNControlBlock("Control Blocks", alaska);
  BTNControlBlock.setPosition({800, 25});

  Button BTNDrawBlock("Draw Blocks", alaska);
  BTNDrawBlock.setPosition({1020, 25});

  Button BTNMotionBlock("Motion", alaska);
  BTNMotionBlock.setPosition({1220, 25});

  // A mini 'tab-bar' switcher.
  auto select_tab = [&BTNControlBlock, &BTNDrawBlock,
                     &BTNMotionBlock](Button &currently_selected_tab_button) {
    // Unfill all the tabs.
    BTNControlBlock.button_fill_color = sf::Color::Green;
    BTNDrawBlock.button_fill_color = sf::Color::Green;
    BTNMotionBlock.button_fill_color = sf::Color::Green;
    // Fill back the selected tab.
    currently_selected_tab_button.button_fill_color = sf::Color(200, 200, 200);
  };

  BTNControlBlock.clicked_callback = [&currently_selected_tab, &BTNControlBlock,
                                      select_tab]() {
    currently_selected_tab = BLOCKS_TAB_NAME::TAB_CONTROL;
    select_tab(BTNControlBlock);
  };

  BTNDrawBlock.clicked_callback = [&currently_selected_tab, &BTNDrawBlock,
                                   select_tab]() {
    currently_selected_tab = BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES;
    select_tab(BTNDrawBlock);
  };

  BTNMotionBlock.clicked_callback = [&currently_selected_tab, &BTNMotionBlock,
                                     select_tab]() {
    currently_selected_tab = BLOCKS_TAB_NAME::TAB_MOTION;
    select_tab(BTNMotionBlock);
  };

  // Just to select that block and fill it with color.
  BTNControlBlock.clicked_callback();

  //'editor blocks' are the blocks found in different tabs,that helps to create
  // new blocks. That new blocks are then added to 'blocks' vector.
  std::vector<Block> editor_blocks;
  editor_blocks.push_back(block_program_started);
  editor_blocks.push_back(block_say);
  editor_blocks.push_back(block_forever);
  editor_blocks.push_back(block_if);
  editor_blocks.push_back(block_key_pressed);

  editor_blocks.push_back(block_go_to_xy);
  editor_blocks.push_back(block_change_x_by);
  editor_blocks.push_back(block_change_y_by);

  editor_blocks.push_back(block_draw_line);
  editor_blocks.push_back(block_draw_circle);
  editor_blocks.push_back(block_draw_rectangle);
  editor_blocks.push_back(block_draw_triangle);

  //'blocks' are user created blocks.
  std::vector<Block> blocks;
  // blocks.at(3).attach_block_next(&blocks.at(4));

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
        generate_code(blocks, sprite_name.get_text());
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Middle) {
        middle_click = true;
      }

      sprite_name.handle_inputs(event);

      BTNControlBlock.handle_inputs(event);
      BTNDrawBlock.handle_inputs(event);
      BTNMotionBlock.handle_inputs(event);

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

        // Control Blocks don't attach to anything.
        if (current_dragging_block_ref->is_control_block()) {
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

        block.process_inside_snap_hints(attach_block_requested,
                                        current_dragging_block_ref);
      }
    }

    window.draw(blocks_tab);
    BTNControlBlock.Render();
    BTNDrawBlock.Render();
    BTNMotionBlock.Render();

    sf::Vector2f block_in_tabs_draw_position =
        blocks_tab.getPosition() + sf::Vector2f(50, 50);

    // These are editor blocks which are for spawning new blocks.
    for (auto &block : editor_blocks) {
      if (block.TabItBelongsToName != currently_selected_tab) {
        continue;
      }

      block.set_position(block_in_tabs_draw_position);
      // block._recalculate_rect();
      block.Render();
      // window.draw(block.block_rect);
      block_in_tabs_draw_position.y += 100;
      // TODO:maybe use + _next_block_snap_rect() but that iterates all over
      // it's child use that once its cached.
    }

    if (!is_any_block_being_dragged &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      for (auto block : editor_blocks) {
        if (block.TabItBelongsToName != currently_selected_tab) {
          continue;
        }

        // Spawn New Block.
        if (isMouseOverSprite(block.block_rect)) {
          // auto spawned_block = GET_BOUND_BLOCK_FN("block_forever").value()();
          auto spawned_block =
              GET_BOUND_BLOCK_FN(block.function_identifier).value()();
          spawned_block.set_position((sf::Vector2f)mouse_position);
          spawned_block.dragging = true;
          blocks.push_back(spawned_block);
          break;
        }
      }
    }

    // These are user defined blocks.
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

    sprite_name.Render();

    window.display();
  }

  return 0;
}
