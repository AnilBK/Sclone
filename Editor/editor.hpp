#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../Globals.hpp"
#include "../TransformGizmo2D.hpp"
#include "../UI/Container.hpp"
#include "../UI/Label.hpp"
#include "../UI/UIButton.hpp"
#include "../UI/UILineInput.hpp"
#include "../UI/UISprite.hpp"
#include <memory>
#include <vector>

// Individual Sprites Added by the User.
class EditorSprite {
public:
  int id = -1; //-1 means Invalid.
  std::string name;
  sf::Vector2f position;
  bool visibility;
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

  HBoxContainer first_line;
  HBoxContainer third_line;
  VBoxContainer editor_inspector;

private:
  // Add new sprite section.
  UILineInput new_sprite_name_input = UILineInput("Sprite n");
  UIButton add_new_sprite_btn = UIButton("+");
  HBoxContainer new_sprite_hbox;

  VBoxContainer user_added_sprites_list;
  TransformGizmo2D gizmo_2D;

  std::vector<std::shared_ptr<UIButton>> user_added_sprite_ptrs;

  std::vector<std::shared_ptr<sf::Texture>> textures;

  // This will act as an unique id provided to the sprites created by the user.
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

public:
  std::vector<EditorSprite> user_added_sprites;

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

    editor_inspector.add_child(first_line);
    editor_inspector.add_child(sprite_pos);
    editor_inspector.add_child(third_line);
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

    add_new_sprite("First Sprite");
    add_new_sprite("Second Sprite");

    select_sprite_by_id(0);
    // gizmo_2D.setTargetSprite(&sprites.at(0));
  }

  EditorSprite *_currently_selected_sprite_ptr();
  void handle_inputs(sf::Event event);
  void Render();
};

#endif
