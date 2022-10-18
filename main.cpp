#include "Editor/Editor.hpp"
#include "Editor/Script.hpp"
#include "Globals.hpp"
#include "UI/UIButton.hpp"
#include "block/Block.hpp"
#include "block/BuiltInBlocks.hpp"
#include "block/NODEBaseClass.hpp"
#include "block/TabBar.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SHOW_FPS
#define SHOW_MOUSE_POS

unsigned int height = 800;
unsigned int width = 800;

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
using Block_fn = std::function<void(Block *)>;
std::vector<std::pair<std::string, Block_fn>> bound_blocks;

// Bound a function pointer to the given string.
void BIND_BLOCK(std::string bind_string, Block_fn fn_that_returns_the_block) {
  for (const auto &b_block : bound_blocks) {
    auto b_bind_string = b_block.first;
    ERR_FAIL_COND_CRASH(b_bind_string == bind_string,
                        "[Debug] String \"" + bind_string +
                            "\" Previously bound to some function.");
  }

  bound_blocks.push_back({bind_string, fn_that_returns_the_block});
}

// A 'Block_fn' returns the block.
// The returned block contains the function identifer string which identifies
// that block. Bind that return string to the function that generated the block
// in the first place.
void BIND_BLOCK(Block_fn fn_that_returns_the_block) {
  Block *test_block = new Block();
  fn_that_returns_the_block(test_block);
  auto bind_string = test_block->function_identifier;
  delete test_block;
  BIND_BLOCK(bind_string, fn_that_returns_the_block);

  // auto bind_string = fn_that_returns_the_block().function_identifier;
  // BIND_BLOCK(bind_string, fn_that_returns_the_block);
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
  ERR_FAIL_COND_CRASH(true, unbound_msg_str);

  return {};
}

void spawn_and_bind_editor_blocks(std::vector<Block> &to) {
  auto SPAWN_EDITOR_BLOCK_AND_BIND = [&to](const Block_fn &fn_ptr) {
    std::cout << "Spawn And Bind.\n";
    Block block;
    fn_ptr(&block);
    to.push_back(block);
    BIND_BLOCK(block.function_identifier, fn_ptr);
    std::cout << "[Done]Spawn And Bind.\n";
  };

  to.reserve(25);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_program_started);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_sprite_clicked);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_forever);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_key_pressed);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_sprite_touching);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_if);

  // There are two ways of adding character controller for a script.
  // By Using the editor block.
  // The caveat is that it should be placed in forever block(Recommended).
  // Another method is by using the editors "Add Controller" button, which
  // silently writes the controller code during export, but the speed of this
  // movement isn't configurable and defaults to 200.
  SPAWN_EDITOR_BLOCK_AND_BIND(
      BUILT_IN_BLOCKS::block_default_character_controller);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_go_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_x_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_y_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_point_to_point);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_say);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_line);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_circle);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_rectangle);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_triangle);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_create_int);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_set_int);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_create_float);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_set_float);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_create_string);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_set_string);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_create_vector2f);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_set_vector2f);
}

void bind_block_generators() {
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
    BIND_BLOCK("block_sprite_clicked", BUILT_IN_BLOCKS::block_sprite_clicked);

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

  std::cout << "Binding Functions:\n";
  BIND_BLOCK(BUILT_IN_BLOCKS::block_say);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_program_started);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_forever);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_if);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_key_pressed);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_sprite_clicked);

  BIND_BLOCK(BUILT_IN_BLOCKS::block_go_to_xy);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_change_x_by);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_change_y_by);

  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_line);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_circle);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_rectangle);
  BIND_BLOCK(BUILT_IN_BLOCKS::block_draw_triangle);
}

void add_editor_blocks(std::vector<Block> &to) {
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

  std::cout << "Creating Editor Blocks:\n";
  to.reserve(15);

#define SPAWN_BLOCK(name, fn_ptr)                                              \
  Block name;                                                                  \
  fn_ptr(&name);                                                               \
  to.push_back(name);

  SPAWN_BLOCK(block_say, BUILT_IN_BLOCKS::block_say);
  SPAWN_BLOCK(block_program_started, BUILT_IN_BLOCKS::block_program_started);
  SPAWN_BLOCK(block_sprite_clicked, BUILT_IN_BLOCKS::block_sprite_clicked);
  SPAWN_BLOCK(block_forever, BUILT_IN_BLOCKS::block_forever);
  SPAWN_BLOCK(block_key_pressed, BUILT_IN_BLOCKS::block_key_pressed);
  SPAWN_BLOCK(block_if, BUILT_IN_BLOCKS::block_if);

  SPAWN_BLOCK(block_go_to_xy, BUILT_IN_BLOCKS::block_go_to_xy);
  SPAWN_BLOCK(block_change_x_by, BUILT_IN_BLOCKS::block_change_x_by);
  SPAWN_BLOCK(block_change_y_by, BUILT_IN_BLOCKS::block_change_y_by);

  SPAWN_BLOCK(block_draw_line, BUILT_IN_BLOCKS::block_draw_line);
  SPAWN_BLOCK(block_draw_circle, BUILT_IN_BLOCKS::block_draw_circle);
  SPAWN_BLOCK(block_draw_rectangle, BUILT_IN_BLOCKS::block_draw_rectangle);
  SPAWN_BLOCK(block_draw_triangle, BUILT_IN_BLOCKS::block_draw_triangle);

#undef SPAWN_BLOCK
}

int main() {
  init_global_font_and_label();

  sf::Font alaska;
  ERR_FAIL_COND_CRASH(!alaska.loadFromFile("alaska.ttf"),
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

  const auto window_clear_color = sf::Color(153, 195, 180);

  // bind_block_generators();
  // std::cout << "[Done]Binding Functions:\n\n";

  //'editor blocks' are the blocks found in different tabs,that helps to create
  // new blocks. That new blocks are then added to 'blocks' vector.
  std::vector<Block> editor_blocks;
  // editor_blocks.reserve(bound_blocks.size());
  spawn_and_bind_editor_blocks(editor_blocks);
  // add_editor_blocks(editor_blocks);
  //  This debug is here so that we can see all objects being destroyed after
  //  the function call.
  std::cout << "[Done]Creating Editor Blocks:\n\n";

  Editor editor;

  auto tab_pos = sf::Vector2f(800, 0);
  auto tab_size = sf::Vector2f(width - tab_pos.x, height - tab_pos.y);
  TabBar built_in_blocks_tab_bar(tab_pos, tab_size);
  built_in_blocks_tab_bar.set_spacing_between_tab_btns(0.0f); // 5.0f

  UIButton blocks_tab_bar_collapse_btn("V");
  blocks_tab_bar_collapse_btn.is_flat = false;
  blocks_tab_bar_collapse_btn.setPosition(
      tab_pos - sf::Vector2f(blocks_tab_bar_collapse_btn.rect_size().x, 0));

  std::function<void()> toggle_tab_bar_folding = [&blocks_tab_bar_collapse_btn,
                                                  &built_in_blocks_tab_bar]() {
    auto &render_status = built_in_blocks_tab_bar.render_status;
    if (render_status == TabBarStatus::SHOW_ALL) {
      // Gray color,to indicate the button is disabled.
      blocks_tab_bar_collapse_btn.button_fill_color = sf::Color(200, 200, 200);
      render_status = TabBarStatus::SHOW_ONLY_TITLE;
    } else {
      blocks_tab_bar_collapse_btn.button_fill_color = sf::Color::Green;
      render_status = TabBarStatus::SHOW_ALL;
    }
  };

  blocks_tab_bar_collapse_btn.clicked_callback = toggle_tab_bar_folding;

  built_in_blocks_tab_bar.add_tab("Control Blocks");
  built_in_blocks_tab_bar.add_tab("Draw Blocks");
  built_in_blocks_tab_bar.add_tab("Motion");
  built_in_blocks_tab_bar.add_tab("Vars..");
  built_in_blocks_tab_bar._select_tab(0);
  built_in_blocks_tab_bar.recalculate_post_add_tabs();

  while (window.isOpen()) {
    bool middle_click = false;

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyReleased &&
           event.key.code == sf::Keyboard::Escape)) {
        window.close();
      } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::LAlt) {
          std::cout << "Code Generated.\n";
          generate_code(editor);
        } else if (event.key.code == sf::Keyboard::Slash) {
          toggle_tab_bar_folding();
        }
      } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
          middle_click = true;
        } else if (event.mouseButton.button == sf::Mouse::Button::XButton1) {
          // The bottom button on left side of a gaming mouse.
          toggle_tab_bar_folding();
        }
      }
      built_in_blocks_tab_bar.handle_inputs(event);
      blocks_tab_bar_collapse_btn.handle_inputs(event);
      editor.handle_inputs(event);
    }

    mouse_position = sf::Mouse::getPosition(window);

    window.clear(window_clear_color);

    editor.Render();

    built_in_blocks_tab_bar.Render();
    blocks_tab_bar_collapse_btn.Render();

    sf::Vector2f block_in_tabs_draw_position =
        built_in_blocks_tab_bar.get_visible_tab_position() +
        sf::Vector2f(50,
                     50 + built_in_blocks_tab_bar.get_scroll_value() * 20.0f);

    int currently_selected_tab = built_in_blocks_tab_bar.currently_selected_tab;

    bool is_any_block_being_dragged =
        editor.script_editor.is_any_block_dragging();
    bool can_spawn_editor_block = !is_any_block_being_dragged &&
                                  sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (built_in_blocks_tab_bar.render_status !=
        TabBarStatus::SHOW_ONLY_TITLE) {
      // These are editor blocks which are for spawning new blocks.
      for (auto &block : editor_blocks) {
        if (block.TabItBelongsToName != currently_selected_tab) {
          continue;
        }

        block.set_position(block_in_tabs_draw_position);
        // block._recalculate_rect();

        // Scrolling above the tab.
        if (block_in_tabs_draw_position.y >=
            built_in_blocks_tab_bar.get_visible_tab_position().y) {
          block.Render();

          // Select the blocks only that are visible.
          if (can_spawn_editor_block) {
            // Spawn New Block.
            if (isMouseOverSprite(block.block_rect)) {
              std::cout << "User Adding a Block.\n";
              Block_fn fn_ptr =
                  GET_BOUND_BLOCK_FN(block.function_identifier).value();

              Block new_block;
              fn_ptr(&new_block);
              new_block.set_position((sf::Vector2f)mouse_position);
              new_block.dragging = true;
              // blocks.push_back(new_block);
              editor.add_block_to_script(new_block);

              // Hide the scripts tab, so we make space for new blocks to
              // spawn. Totally not needed, but as of now, newly spawned
              // blocks render below the tab bar. So, this is kinda hack.
              toggle_tab_bar_folding();

              std::cout << "[Done]User Adding a Block.\n\n";
              can_spawn_editor_block = false;
            }
          }
        }

        // window.draw(block->block_rect);
        float height_of_cur_block = block.block_full_size.y;
        block_in_tabs_draw_position.y += height_of_cur_block + 20;
      }
    }

#ifdef SHOW_FPS
    show_fps_btn.setString("FPS: " +
                           std::to_string(static_cast<int>(get_fps())));
    window.draw(show_fps_btn);
#endif

#ifdef SHOW_MOUSE_POS
    show_mouse_pos_text.setString("X: " + std::to_string(mouse_position.x) +
                                  " Y: " + std::to_string(mouse_position.y));
    window.draw(show_mouse_pos_text);
#endif

    // editor.Render();

    window.display();
  }

  std::cout << "\nProgram Terminated.\n\n";

  return 0;
}
