#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../CodeGen/CodeGenerator.hpp"
#include "../Globals.hpp"
#include "../UI/Container.hpp"
#include "../UI/Label.hpp"
#include "../UI/TabBar.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UILineInput.hpp"
#include "../UI/UISprite.hpp"
#include "../block/Block.hpp"
#include "../block/BlockBinder.hpp"
#include "../block/BuiltInBlocks.hpp"
#include "ScriptEditor.hpp"
#include "TransformGizmo2D.hpp"
#include <memory>
#include <vector>

// Individual Sprites Added by the User.
class EditorSprite {
public:
  int id = -1; //-1 means Invalid.
  int layer = 0;
  std::string name;
  sf::Vector2f position;
  bool visibility;
  bool add_movement_script = false;
  std::string texture;
  sf::Sprite sprite;
};

class Editor {

private:
  // 2D World.
  /// @brief 2D World, where the sprites are seen.
  ///  This contains a freely draggable view.
  ///  https://www.sfml-dev.org/tutorials/2.5/graphics-view.php
  sf::View view;
  sf::FloatRect world;

  /// @brief The lines that show X-axis and Y-axis inside the 2D world.
  sf::RectangleShape grid_x_axis_line, grid_y_axis_line, world2d_border;
  /// @brief Mouse tracking for world panning.
  sf::Vector2f old_mouse_pos, new_mouse_pos;

  // Inspector.
  UILineInput sprite_name = UILineInput("Cat");
  // UIButton sprite_visibility = UIButton("Visible"); Not Used At the moment.
  Label sprite_pos = Label("Position: X: Y:");
  Label sprite_texture = Label("Texture:");
  UILineInput sprite_texture_name = UILineInput("cat.png");

  Label sprite_layer_label = Label("Layer:");
  UILineInput sprite_layer_value_input = UILineInput("0");
  UIButton sprite_move_layer_up = UIButton("+");
  UIButton sprite_move_layer_down = UIButton("-");

  HBoxContainer first_line;
  HBoxContainer third_line;
  HBoxContainer fourth_line;
  VBoxContainer editor_inspector;

  // 'More' button:
  //    to add more functionalities to a sprite.
  UIButton show_more_options_btn = UIButton("V");
  UIButton add_movement_btn = UIButton("Add Movement Script");
  HBoxContainer show_more_hbox;

private:
  // Add new sprite section.
  UILineInput new_sprite_name_input = UILineInput("Sprite");
  UIButton add_new_sprite_btn = UIButton("+");
  HBoxContainer new_sprite_hbox;
  VBoxContainer user_added_sprites_list_parent;
  VBoxContainer user_added_sprites_list_vbox;

  TransformGizmo2D gizmo_2D;

  // The editor spawn blocks section.
  sf::Vector2f tab_pos = sf::Vector2f(800, 0);
  sf::Vector2f tab_size = sf::Vector2f(400, 700);
  TabBar built_in_blocks_tab_bar = TabBar(tab_pos, tab_size);
  UIButton blocks_tab_bar_collapse_btn = UIButton("V");

  UIButton build_and_run_btn = UIButton("Play");

  /// @brief Contains functions that generates different blocks and the strings
  /// that identify that block generator function.
  BlockBinder bound_blocks;

  /// @brief The blocks found in different tabs, that helps to create new blocks
  /// in the editor.
  std::vector<Block> editor_blocks;

  using BtnIDPair = std::pair<std::shared_ptr<UIButton>, int>;
  /// @brief We store an UIButton. This is the button that is shown left on the
  /// list of sprites. We also save an ID of the sprite associated with this
  /// UIButton.
  std::vector<BtnIDPair> btn_id_pairs;

  std::vector<std::shared_ptr<sf::Texture>> textures;

  // This will act as an unique id provided to the sprites created by the
  // user.
  int _total_sprites_added = 0;
  int currently_selected_sprite_id = -1;

  // Textures are loaded using shared_ptrs, so that they are alive throughout
  // the entire program and freed when the program terminates. The pointer
  // to the texture is same throughout the program execution.
  sf::Texture *load_texture(const std::string &texture_file);

  std::string _position_to_string(sf::Vector2f pos);
  void _create_new_sprite();

  void _update_sprite_texure();
  void _update_sprite_name();

  void select_sprite_by_id(int id);
  void _highlight_btn_in_list(const int id);
  void add_new_sprite(const std::string &p_name);

  void _refresh_layout();

  int _selected_sprite_layer();
  void _set_sprite_layer(int new_layer);
  void _increment_sprite_layer();
  void _decrement_sprite_layer();

  void _show_more_btn__show_childrens();
  void _show_more_btn__hide_childrens();
  void _add_movement_script();

  void _spawn_block_at_mouse_pos(const Block &block);
  void _handle_2D_world_inputs(sf::Event event);

  void _build_and_run();

public:
  std::vector<EditorSprite> user_added_sprites;
  std::vector<std::shared_ptr<Script>> scripts;

  ScriptEditor script_editor;

  void _initialize_built_in_blocks_tab_bar() {
    spawn_and_bind_editor_blocks();

    auto win_size = (sf::Vector2f)window.getSize();
    tab_size = win_size - tab_pos;

    built_in_blocks_tab_bar.set_pos(tab_pos);
    built_in_blocks_tab_bar.set_size(tab_size);

    built_in_blocks_tab_bar.set_spacing_between_tab_btns(0.0f); // 5.0f

    blocks_tab_bar_collapse_btn.is_flat = false;
    blocks_tab_bar_collapse_btn.setPosition(
        tab_pos - sf::Vector2f(blocks_tab_bar_collapse_btn.rect_size().x, 0));

    blocks_tab_bar_collapse_btn.clicked_callback = [&]() {
      toggle_tab_bar_folding();
    };

    built_in_blocks_tab_bar.add_tab("Control Blocks");
    built_in_blocks_tab_bar.add_tab("Draw Blocks");
    built_in_blocks_tab_bar.add_tab("Motion");
    built_in_blocks_tab_bar.add_tab("Vars..");
    built_in_blocks_tab_bar._select_tab(0);
    built_in_blocks_tab_bar.recalculate_post_add_tabs();
  }

  Editor() {

    _initialize_built_in_blocks_tab_bar();

    // Stuffs Related To Sprite Inspector.
    sprite_texture_name.enter_pressed_callback = [&]() {
      _update_sprite_texure();
    };
    sprite_name.enter_pressed_callback = [&]() { _update_sprite_name(); };

    first_line.add_child(sprite_name);
    // first_line.add_child(sprite_visibility);

    third_line.add_child(sprite_texture);
    third_line.add_child(sprite_texture_name);

    sprite_move_layer_up.clicked_callback = [&]() {
      _increment_sprite_layer();
    };
    sprite_move_layer_down.clicked_callback = [&]() {
      _decrement_sprite_layer();
    };
    sprite_layer_value_input.enter_pressed_callback = [&]() {
      _set_sprite_layer(_selected_sprite_layer());
      sprite_layer_value_input.line_input_active = false;
    };

    fourth_line.add_child(sprite_layer_label);
    fourth_line.add_child(sprite_move_layer_down);
    fourth_line.add_child(sprite_layer_value_input);
    fourth_line.add_child(sprite_move_layer_up);

    show_more_options_btn.is_flat = false;
    // Disabled at first.
    show_more_options_btn.button_fill_color = sf::Color(200, 200, 200);
    show_more_options_btn.clicked_callback = [&]() {
      if (show_more_options_btn.is_clicked()) {
        // Green color, to indicate the button is enabled.
        show_more_options_btn.button_fill_color = sf::Color::Green;
        _show_more_btn__show_childrens();
      } else {
        // Gray color,to indicate the button is disabled.
        show_more_options_btn.button_fill_color = sf::Color(200, 200, 200);
        _show_more_btn__hide_childrens();
      }
    };

    add_movement_btn.clicked_callback = [&]() { _add_movement_script(); };

    show_more_hbox.add_child(show_more_options_btn);

    editor_inspector.add_child(first_line);
    editor_inspector.add_child(sprite_pos);
    editor_inspector.add_child(third_line);
    editor_inspector.add_child(fourth_line);
    editor_inspector.add_child(show_more_hbox);
    editor_inspector.setPosition({250, 10});

    build_and_run_btn.setPosition(sf::Vector2f(600, 0));
    std::function<void()> build_and_run_func = [this]() { _build_and_run(); };
    build_and_run_btn.clicked_callback = build_and_run_func;

    // Stuffs Related To Add New Sprite Inspector.
    std::function<void()> add_sprite = [this]() { _create_new_sprite(); };
    add_new_sprite_btn.clicked_callback = add_sprite;
    new_sprite_name_input.enter_pressed_callback = add_sprite;
    new_sprite_name_input.is_flat = false;

    new_sprite_hbox.add_child(new_sprite_name_input);
    new_sprite_hbox.add_child(add_new_sprite_btn);
    user_added_sprites_list_parent.add_child(new_sprite_hbox);
    user_added_sprites_list_parent.add_child(user_added_sprites_list_vbox);
    user_added_sprites_list_parent.setPosition({15, 110});

    // add_new_sprite("StarFish");
    add_new_sprite("Cat");

    select_sprite_by_id(0);
    // gizmo_2D.setTargetSprite(&sprites.at(0));

    script_editor.script = selected_script_ptr();

    // Set up the 2D world.
    world = sf::FloatRect(sf::Vector2f(0, 305), sf::Vector2f(700, 455));
    view = sf::View({0, 0, 1200, 800});

    view.setViewport(
        {world.left / window.getSize().x, world.top / window.getSize().y,
         world.width / window.getSize().x, world.height / window.getSize().y});

    view.move({-60.0, -60.0});

    grid_x_axis_line.setPosition(sf::Vector2f());
    grid_x_axis_line.setSize({static_cast<float>(window.getSize().x), 5});
    grid_x_axis_line.setFillColor(sf::Color::Red);

    grid_y_axis_line.setPosition(sf::Vector2f());
    grid_y_axis_line.setSize({5, static_cast<float>(window.getSize().y)});
    grid_y_axis_line.setFillColor(sf::Color::Green);

    world2d_border.setPosition({world.left, world.top});
    world2d_border.setSize({world.width, world.height});
    world2d_border.setFillColor(sf::Color::Cyan);
    world2d_border.setOutlineColor(sf::Color(130, 130, 130));
    world2d_border.setOutlineThickness(3.0f);
  }

  void spawn_and_bind_editor_blocks();

  void add_block_to_script(Block b);

  Script *selected_script_ptr();
  EditorSprite *selected_sprite_ptr();
  Script *get_script_attached_to_editor_sprite(EditorSprite *sprite);

  std::vector<const EditorSprite *> get_sprites_sorted_by_layers() const;

  void handle_inputs(sf::Event event);
  void _render_bounding_box_over_selected_sprite();
  void _render_ui();
  void _render_sprites();
  void _process_2D_gizmo();

  void toggle_tab_bar_folding();

  void _render_tab();

  void Render();
};

#endif
