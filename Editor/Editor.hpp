#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../Globals.hpp"
#include "../UI/Container.hpp"
#include "../UI/Label.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UILineInput.hpp"
#include "../UI/UISprite.hpp"
#include "../block/Block.hpp"
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
  // Points to the button that is related with this sprite.
  UIButton *ui_btn_ref = nullptr;
  sf::Sprite sprite;
};

class Editor {

private:
  // Inspector.
  UILineInput sprite_name = UILineInput("Cat");
  UIButton sprite_visibility = UIButton("Visible");
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
  UILineInput new_sprite_name_input = UILineInput("Sprite n");
  UIButton add_new_sprite_btn = UIButton("+");
  HBoxContainer new_sprite_hbox;

  VBoxContainer user_added_sprites_list;
  TransformGizmo2D gizmo_2D;

  std::vector<std::shared_ptr<UIButton>> user_added_sprite_ptrs;

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
  void _highlight_selected_btn_in_list(const UIButton *btn_to_highlight);
  void add_new_sprite(const std::string &p_name);

  void _refresh_layout();
  void _render_ui();
  void _render_sprites();
  void _process_2D_gizmo();

  int _selected_sprite_layer();
  void _set_sprite_layer(int new_layer);
  void _increment_sprite_layer();
  void _decrement_sprite_layer();

  void _show_more_btn__show_childrens();
  void _show_more_btn__hide_childrens();
  void _add_movement_script();

public:
  std::vector<EditorSprite> user_added_sprites;
  std::vector<std::shared_ptr<Script>> scripts;

  ScriptEditor script_editor;

  Editor() {
    // Stuffs Related To Sprite Inspector.
    sprite_texture_name.enter_pressed_callback = [&]() {
      _update_sprite_texure();
    };
    sprite_name.enter_pressed_callback = [&]() { _update_sprite_name(); };

    first_line.add_child(sprite_name);
    first_line.add_child(sprite_visibility);

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
    fourth_line.add_child(sprite_layer_value_input);
    fourth_line.add_child(sprite_move_layer_down);
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

    // Stuffs Related To Add New Sprite Inspector.
    std::function<void()> add_sprite = [this]() { _create_new_sprite(); };
    add_new_sprite_btn.clicked_callback = add_sprite;
    new_sprite_name_input.enter_pressed_callback = add_sprite;
    new_sprite_name_input.is_flat = false;

    new_sprite_hbox.add_child(new_sprite_name_input);
    new_sprite_hbox.add_child(add_new_sprite_btn);
    user_added_sprites_list.add_child(new_sprite_hbox);
    user_added_sprites_list.setPosition({15, 110});

    add_new_sprite("StarFish");
    add_new_sprite("Cat");

    select_sprite_by_id(0);
    // gizmo_2D.setTargetSprite(&sprites.at(0));

    script_editor.script = selected_script_ptr();
  }

  void add_block_to_script(Block b);

  Script *selected_script_ptr();
  EditorSprite *selected_sprite_ptr();
  Script *get_script_attached_to_editor_sprite(EditorSprite *sprite);

  std::vector<const EditorSprite *> get_sprites_sorted_by_layers() const;

  void handle_inputs(sf::Event event);
  void Render();
};

#endif