#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../CodeGen/CodeGenerator.hpp"
#include "../Globals.hpp"
#include "../Nodes/Node.hpp"
#include "../UI/Container.hpp"
#include "../UI/TabBar.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UILabel.hpp"
#include "../UI/UILineInput.hpp"
#include "../UI/UISprite.hpp"
#include "../UI/UITreeView.h"
#include "../block/Block.hpp"
#include "../block/BlockBinder.hpp"
#include "../block/BuiltInBlocks.hpp"
#include "BlockVisualizer.hpp"
#include "EditorSprite.hpp"
#include "PropertiesListUI.hpp"
#include "ScriptEditor.hpp"
#include "TransformGizmo2D.hpp"
#include "Windows/GlobalAlertWindow.hpp"
#include <map>
#include <memory>
#include <vector>

class BlockVisualizer;

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
  UILabel sprite_pos = UILabel("Position: X: Y:");
  UILabel sprite_texture = UILabel("Texture:");
  UILineInput sprite_texture_name = UILineInput("cat.png");

  UILabel sprite_layer_label = UILabel("Layer:");
  UILineInput sprite_layer_value_input = UILineInput("0");
  UIButton sprite_move_layer_up = UIButton("+");
  UIButton sprite_move_layer_down = UIButton("-");

  HBoxContainer first_line;
  HBoxContainer third_line;
  HBoxContainer fourth_line;
  VBoxContainer editor_inspector;

  UIDropDown add_node_type_drop_down =
      UIDropDown({"Sprite", "CircleShape", "RectangleShape"});

  // 'More' button:
  //    to add more functionalities to a sprite.
  UIButton show_more_options_btn = UIButton("V");
  UIButton add_movement_btn = UIButton("Add Movement Script");
  HBoxContainer show_more_hbox;

  sf::Clock frameClock;

private:
  // Add new sprite section.
  UILineInput new_sprite_name_input = UILineInput("Sprite");
  UIButton add_new_sprite_btn = UIButton("+");

  HBoxContainer new_sprite_hbox;

  TransformGizmo2D gizmo_2D;
  UILabel escape_sprite_dragging_label =
      UILabel("Enter Esc to stop dragging sprite.");

  // The editor spawn blocks section.
  sf::Vector2f tab_pos = sf::Vector2f(700, 0);
  sf::Vector2f tab_size = sf::Vector2f(400, 300);
  TabBar built_in_blocks_tab_bar = TabBar(tab_pos, tab_size);
  UIButton blocks_tab_bar_collapse_btn = UIButton("V");

  // The information and properties tab.
  TabBar info_tab = TabBar(sf::Vector2f(250, 0), sf::Vector2f(300, 300));

  PropertiesListUI properties_list_ui = PropertiesListUI(sf::Vector2f(265, 40));

  UIButton build_and_run_btn = UIButton("Play");

  BlockVisualizer block_visualizer = BlockVisualizer(*this);

  /// @brief Contains functions that generates different blocks and the strings
  /// that identify that block generator function.
  BlockBinder bound_blocks;

  /// @brief The blocks found in different tabs, that helps to create new blocks
  /// in the editor.
  std::vector<Block> editor_blocks;

  struct ListItemSpritePair {
    int list_item_id;
    int sprite_id;
  };

  /// @brief We store pairs of (index assigned to the TreeView
  /// item,sprite_id). In this way we know, which sprite_id is associated with
  /// the TreeView item.
  std::vector<ListItemSpritePair> tree_item_sprite_index_pairs;

  UITreeView sprite_list_ui_tree =
      UITreeView(sf::Vector2f(20, 110), sf::Vector2f(160, 180));

  std::optional<int>
  _get_index_of_btn_related_to_editor_sprite(int p_sprite_id);

  std::vector<std::shared_ptr<sf::Texture>> textures;

  // This will act as an unique id provided to the sprites created by the
  // user.
  int _total_sprites_added = 0;
  int currently_selected_sprite_id = -1;

  /// @brief When a sprite is added, we store their name here and count how many
  /// times it is repeated. So let's say first time we add 'Sprite' and set
  /// count = 0, second time when we press the '+' button, we add "Sprite_1".
  /// (std::string) name of the sprite,
  /// (std::size_t) number of sprites added from this name.
  std::map<std::string, std::size_t> added_sprite_names;

  /// @brief Looks for sprite names. If they don't already exist, then returns
  /// that name, otherwise appends the sprite_name with some index and returns
  /// them. Basically used for duplicating sprites.
  /// @param name Name of the sprite to generate unique name from.
  /// @return Valid sprite name that hasn't already been added.
  std::string get_unique_sprite_name_from(const std::string &name);

  // Textures are loaded using shared_ptrs, so that they are alive throughout
  // the entire program and freed when the program terminates. The pointer
  // to the texture is same throughout the program execution.
  std::optional<sf::Texture *> load_texture(const std::string &texture_file);

  std::string _position_to_string(sf::Vector2f pos);
  void _create_new_sprite();

  void _update_sprite_texure();
  void _update_sprite_name();

  void select_sprite_by_id(int id);
  void _highlight_btn_in_list(const int id);
  void add_new_sprite(const std::string &p_sprite_name);

  void _refresh_layout();

  void _render_sprite_list_ui();
  void _handle_sprite_list_inputs(sf::Event event);

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

  void _on_sprites_translation_update(bool update_translation,
                                      bool update_scale);

  void _init_sprite_list_ui() {
    // Stuffs Related To Add New Sprite Inspector.
    std::function<void()> add_sprite = [this]() { _create_new_sprite(); };
    add_new_sprite_btn.clicked_callback = add_sprite;
    new_sprite_name_input.enter_pressed_callback = add_sprite;
    new_sprite_name_input.is_flat = false;

    new_sprite_hbox.add_child(new_sprite_name_input);
    new_sprite_hbox.add_child(add_new_sprite_btn);
    new_sprite_hbox.setPosition({20, 75});

    // Call init_ui late, because we need window size that hasn't been
    // initialized or sth.. I forgot why i did this earlier.
    //  TODO ?? Figure out.
    sprite_list_ui_tree.init_ui();
  }

public:
  std::vector<EditorSprite> user_added_sprites;
  std::vector<std::shared_ptr<Script>> scripts;

  struct cache {
    EditorSprite *selected_sprite_ptr = nullptr;

    // Sprites sorted by layers (for rendering).
    std::vector<const EditorSprite *> sprites_sorted_by_layers;
    // Sprites reversely sorted by layers (for picking up inputs).
    // We may not need this additional reverse cache, we may just iterate the
    // 'sprites_sorted_by_layers' vector in reverse.
    std::vector<const EditorSprite *> sprites_sorted_by_layers_reverse;
  } Cache;

  void update_sorted_sprites_cache();

  ScriptEditor script_editor;

  void _initialize_built_in_blocks_tab_bar() {
    spawn_and_bind_editor_blocks();

    auto win_size = (sf::Vector2f)window.getSize();
    tab_size.x = win_size.x - tab_pos.x;

    built_in_blocks_tab_bar.set_pos(tab_pos);
    built_in_blocks_tab_bar.set_size(tab_size);

    blocks_tab_bar_collapse_btn.is_flat = false;
    blocks_tab_bar_collapse_btn.setPosition(
        tab_pos - sf::Vector2f(blocks_tab_bar_collapse_btn.rect_size().x, 0));

    blocks_tab_bar_collapse_btn.clicked_callback = [&]() {
      toggle_tab_bar_folding();
    };

    built_in_blocks_tab_bar.add_tab("Control Blocks");
    built_in_blocks_tab_bar.add_tab("Draw Blocks");
    built_in_blocks_tab_bar.add_tab("Motion");
    built_in_blocks_tab_bar.add_tab("Variables");
    built_in_blocks_tab_bar.select_tab(0);
  }

  Editor() {

    _initialize_built_in_blocks_tab_bar();

    info_tab.add_tab("Info");
    info_tab.add_tab("Properties");
    info_tab.select_tab(0);

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
    editor_inspector.setPosition({265, 40});

    add_node_type_drop_down.setPosition({20, 40});

    build_and_run_btn.setPosition(sf::Vector2f(600, 0));
    std::function<void()> build_and_run_func = [this]() { _build_and_run(); };
    build_and_run_btn.clicked_callback = build_and_run_func;

    // add_new_sprite("StarFish");
    add_new_sprite("Cat");

    select_sprite_by_id(0);
    // gizmo_2D.setTargetSprite(&sprites.at(0));

    std::function<void(bool, bool)> translation_func =
        [this](bool update_translation, bool update_scale) {
          _on_sprites_translation_update(update_translation, update_scale);
        };
    gizmo_2D.translation_updated_callbacks = translation_func;

    script_editor.script = selected_script_ptr();

    // Set up the 2D world.
    world = sf::FloatRect(sf::Vector2f(0, 305), sf::Vector2f(700, 455));
    view = sf::View({0, 0, 1200, 800});

    auto win_size = window.getSize();

    view.setViewport({world.left / win_size.x, world.top / win_size.y,
                      world.width / win_size.x, world.height / win_size.y});

    view.move({-60.0, -60.0});

    _init_sprite_list_ui();

    grid_x_axis_line.setPosition(sf::Vector2f());
    grid_x_axis_line.setSize({static_cast<float>(win_size.x), 5});
    grid_x_axis_line.setFillColor(sf::Color::Red);

    grid_y_axis_line.setPosition(sf::Vector2f());
    grid_y_axis_line.setSize({5, static_cast<float>(win_size.y)});
    grid_y_axis_line.setFillColor(sf::Color::Green);

    world2d_border.setPosition(world.getPosition());
    world2d_border.setSize(world.getSize());
    world2d_border.setFillColor(sf::Color(153, 195, 180));
    world2d_border.setOutlineColor(sf::Color(71, 71, 71));
    world2d_border.setOutlineThickness(4.0f);

    escape_sprite_dragging_label.setPosition(world.getPosition() +
                                             sf::Vector2f(10, 5));

    // World 2D and Script Editor fills the whole screen.
    auto script_editor_pos = sf::Vector2f(
        world.left + world.width + world2d_border.getOutlineThickness(),
        world.top);

    auto script_editor_size =
        sf::Vector2f(win_size.x - script_editor_pos.x, world.height);

    script_editor.setRect({script_editor_pos, script_editor_size});

    // Set up global references to editor views.
    editor_view = get_world_2d_view_ptr();
    block_visualizer.init();
  }

  void spawn_and_bind_editor_blocks();

  void add_block_to_script(Block b);

  Script *selected_script_ptr();
  Script *get_script_attached_to_editor_sprite(EditorSprite *sprite);

  // Returns the selected sprite ptr.
  EditorSprite *_selected_sprite_ptr();
  // Get the cached pointer to the selected sprite.
  EditorSprite *selected_sprite_ptr() { return Cache.selected_sprite_ptr; }

  std::vector<const EditorSprite *> get_sprites_sorted_by_layers() const;

  void handle_inputs(sf::Event event);
  void _render_bounding_box_over_selected_sprite();
  void _render_ui();
  void _render_sprites();
  void _process_2D_gizmo();
  void _pick_sprite();

  void toggle_tab_bar_folding();

  void _render_block_spawner_tab();

  sf::View *get_world_2d_view_ptr() { return &view; }

  void Render();
};

#endif
