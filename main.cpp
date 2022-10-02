#include "BuiltInBlocks.hpp"
#include "Globals.hpp"
#include "TransformGizmo2D.hpp"
#include "block/Block.hpp"
#include "block/Button.hpp"
#include "block/LineInput.hpp"
#include "block/NODEBaseClass.hpp"
#include "block/TabBar.hpp"
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

  to.reserve(20);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_say);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_program_started);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_forever);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_if);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_key_pressed);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_sprite_clicked);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_go_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_x_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_y_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_point_to_point);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_line);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_circle);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_rectangle);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_triangle);
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
  SPAWN_BLOCK(block_forever, BUILT_IN_BLOCKS::block_forever);
  SPAWN_BLOCK(block_if, BUILT_IN_BLOCKS::block_if);
  SPAWN_BLOCK(block_key_pressed, BUILT_IN_BLOCKS::block_key_pressed);
  SPAWN_BLOCK(block_sprite_clicked, BUILT_IN_BLOCKS::block_sprite_clicked);

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

  std::string editor_sprite_texture_file_name = "cat.png";

  sf::Texture editor_sprite_texture;
  ERR_FAIL_COND_CRASH(
      !editor_sprite_texture.loadFromFile(editor_sprite_texture_file_name),
      "Error while loading texture.");
  editor_sprite_texture.setSmooth(true);

  auto editorSpriteTextureSize = (sf::Vector2f)editor_sprite_texture.getSize();

  // TODO: Make a container like Block, which handles all the resizing.
  sf::Sprite sprite_preview;
  sprite_preview.setTexture(editor_sprite_texture);
  sprite_preview.setScale(128 / editorSpriteTextureSize.x,
                          128 / editorSpriteTextureSize.y);
  sprite_preview.setPosition(250.0f, 10.0f);

  LineInput sprite_name;
  sprite_name.position =
      sprite_preview.getPosition() + sf::Vector2f(128 + 32.0f, 0.0f);
  sprite_name.input_text = "cat";
  sprite_name.line_input_active = false;

  sf::Sprite editor_sprite;
  editor_sprite.setTexture(editor_sprite_texture);
  sf::FloatRect catSize = editor_sprite.getGlobalBounds();
  editor_sprite.setOrigin(catSize.width / 2.0f, catSize.height / 2.0f);
  editor_sprite.setPosition(window.getSize().x / 2.0f,
                            window.getSize().y / 2.0f);

  bool sprite_visible = true;

  TransformGizmo2D gizmo_2D;
  gizmo_2D.setTargetSprite(&editor_sprite);

  // TODO: Should be a checkbox.
  Button BTN_sprite_visible("Visible", font);
  BTN_sprite_visible.setPosition(
      sprite_name.position +
      sf::Vector2f(sprite_name.rect_size().x + 10, -10.0f));
  BTN_sprite_visible.button_fill_color = sf::Color::Green;
  std::function<void()> toggle_visibility = [&BTN_sprite_visible,
                                             &sprite_visible, &gizmo_2D]() {
    if (BTN_sprite_visible.get_text() == "Visible") {
      sprite_visible = false;
      BTN_sprite_visible.setLabel("Not Visible");
      BTN_sprite_visible.button_fill_color = sf::Color(153, 153, 102);
      gizmo_2D._undrag_gizmos();
    } else {
      sprite_visible = true;
      BTN_sprite_visible.setLabel("Visible");
      BTN_sprite_visible.button_fill_color = sf::Color::Green;
    }
  };
  BTN_sprite_visible.clicked_callback = toggle_visibility;

  sf::Text sprite_pos_text;
  sprite_pos_text.setFont(font);
  sprite_pos_text.setPosition(sprite_name.position + sf::Vector2f(0, 50));
  sprite_pos_text.setFillColor(sf::Color::Black);
  auto update_sprite_pos_text = [&sprite_pos_text, &editor_sprite]() {
    sprite_pos_text.setString(
        "X: " + std::to_string((int)editor_sprite.getPosition().x) +
        " Y: " + std::to_string((int)editor_sprite.getPosition().y));
  };
  update_sprite_pos_text();

  sf::Text sprite_texture_name_label;
  sprite_texture_name_label.setFont(font);
  sprite_texture_name_label.setString("Texture:");
  sprite_texture_name_label.setPosition(sprite_pos_text.getPosition() +
                                        sf::Vector2f(0, 40));
  sprite_texture_name_label.setFillColor(sf::Color::Black);

  float texture_label_size_x =
      sprite_texture_name_label.getGlobalBounds().width;

  LineInput texture_file_name;
  texture_file_name.position = sprite_texture_name_label.getPosition() +
                               sf::Vector2f(texture_label_size_x + 10, 0.0f);
  texture_file_name.input_text = editor_sprite_texture_file_name;
  texture_file_name.line_input_active = false;

  Button BTN_reload_texture("Reload", font);
  BTN_reload_texture.setPosition(
      texture_file_name.position +
      sf::Vector2f(texture_file_name.rect_size().x + 10, 0.0f));
  BTN_reload_texture.button_fill_color = sf::Color::Green;
  std::function<void()> reload_texture = [&editor_sprite_texture,
                                          &texture_file_name, &sprite_preview,
                                          &editor_sprite]() {
    if (!editor_sprite_texture.loadFromFile(texture_file_name.get_text())) {
      std::cout << "Error while loading texture.";
      exit(1);
    }
    sprite_preview.setTexture(editor_sprite_texture);
    editor_sprite.setTexture(editor_sprite_texture);
    // TODO:Check this scaling.
    // Resize these sprites to match the size of the new texture.
    auto editorSpriteTextureSize =
        (sf::Vector2f)editor_sprite_texture.getSize();
    sprite_preview.setScale(128 / editorSpriteTextureSize.x,
                            128 / editorSpriteTextureSize.y);
  };

  BTN_reload_texture.clicked_callback = reload_texture;

  // Update the size,as length of texture file name changes.
  auto re_position_BTN_reload_texture = [&BTN_reload_texture,
                                         &texture_file_name]() {
    BTN_reload_texture.setPosition(
        texture_file_name.position +
        sf::Vector2f(texture_file_name.rect_size().x + 10, -10.0f));
  };

  auto re_position_BTN_sprite_visible = [&BTN_sprite_visible, &sprite_name]() {
    BTN_sprite_visible.setPosition(
        sprite_name.position +
        sf::Vector2f(sprite_name.rect_size().x + 10, -10.0f));
  };

  texture_file_name.connect("resized", re_position_BTN_reload_texture);
  sprite_name.connect("resized", re_position_BTN_sprite_visible);

  // bind_block_generators();
  std::cout << "[Done]Binding Functions:\n\n";

  //'editor blocks' are the blocks found in different tabs,that helps to create
  // new blocks. That new blocks are then added to 'blocks' vector.
  std::vector<Block> editor_blocks;
  // editor_blocks.reserve(bound_blocks.size());
  spawn_and_bind_editor_blocks(editor_blocks);
  // add_editor_blocks(editor_blocks);
  //  This debug is here so that we can see all objects being destroyed after
  //  the function call.
  std::cout << "[Done]Creating Editor Blocks:\n\n";

  //'blocks' are user created blocks.
  std::vector<Block> blocks;
  std::cout << "Block = " << sizeof(Block) << " bytes.\n";
  blocks.reserve(16);
  // blocks.at(3).attach_block_next(&blocks.at(4));

  auto tab_pos = sf::Vector2f(800, 25);
  auto tab_size = sf::Vector2f(width - tab_pos.x, height - tab_pos.y);
  TabBar built_in_blocks_tab_bar(tab_pos, tab_size);

  built_in_blocks_tab_bar.add_tab("Control Blocks");
  built_in_blocks_tab_bar.add_tab("Draw Blocks");
  built_in_blocks_tab_bar.add_tab("Motion");
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
      } else if (event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::LAlt) {
        generate_code(blocks, sprite_name.get_text(),
                      texture_file_name.get_text(),
                      editor_sprite.getPosition());
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Middle) {
        middle_click = true;
      }

      sprite_name.handle_inputs(event);
      BTN_sprite_visible.handle_inputs(event);
      texture_file_name.handle_inputs(event);
      BTN_reload_texture.handle_inputs(event);

      built_in_blocks_tab_bar.handle_inputs(event);

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
        if (block.next_block == nullptr) {
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

    built_in_blocks_tab_bar.Render();

    sf::Vector2f block_in_tabs_draw_position =
        built_in_blocks_tab_bar.get_visible_tab_position() +
        sf::Vector2f(50,
                     50 + built_in_blocks_tab_bar.get_scroll_value() * 20.0f);

    int currently_selected_tab = built_in_blocks_tab_bar.currently_selected_tab;

    bool can_spawn_editor_block = !is_any_block_being_dragged &&
                                  sf::Mouse::isButtonPressed(sf::Mouse::Left);

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

            Block b;
            fn_ptr(&b);
            b.set_position((sf::Vector2f)mouse_position);
            b.dragging = true;
            blocks.push_back(b);

            std::cout << "[Done]User Adding a Block.\n\n";
            can_spawn_editor_block = false;
          }
        }
      }

      // window.draw(block->block_rect);
      float height_of_cur_block = block.block_full_size.y;
      block_in_tabs_draw_position.y += height_of_cur_block + 20;
    }

    // These are user defined blocks.
    for (auto &block : blocks) {
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

    // Info about the currently selected sprites stuffs.
    sprite_name.Render();
    window.draw(sprite_preview);
    BTN_sprite_visible.Render();
    window.draw(sprite_pos_text);
    window.draw(sprite_texture_name_label);
    texture_file_name.Render();
    BTN_reload_texture.Render();

    if (sprite_visible) {
      window.draw(editor_sprite);
      // 2D Gizmo Stuffs.
      gizmo_2D.Render();
      update_sprite_pos_text();
    }

    window.display();
  }

  std::cout << "\nProgram Terminated.\n\n";

  return 0;
}
