#include "Editor.hpp"
#include <cstdlib>

int Editor::_selected_sprite_layer() {
  int layer_value = std::stoi(sprite_layer_value_input.text.getText());
  return layer_value;
}

void Editor::_set_sprite_layer(int new_layer) {
  std::string new_layer_value = std::to_string(new_layer);
  sprite_layer_value_input.set_text(new_layer_value);

  auto *selected_sprite = selected_sprite_ptr();
  if (selected_sprite == nullptr) {
    return;
  }

  selected_sprite->layer = new_layer;
}

void Editor::_increment_sprite_layer() {
  auto curr_layer = _selected_sprite_layer();
  curr_layer++;
  _set_sprite_layer(curr_layer);
}

void Editor::_decrement_sprite_layer() {
  auto curr_layer = _selected_sprite_layer();
  curr_layer--;
  _set_sprite_layer(curr_layer);
}

void Editor::_show_more_btn__show_childrens() {
  show_more_hbox.add_child(add_movement_btn);
}

void Editor::_show_more_btn__hide_childrens() {
  if (show_more_hbox.children.size() < 2) {
    return;
  }
  // We have a single child only, so remove that.
  show_more_hbox.children.erase(show_more_hbox.children.begin() + 1);
  // TODO??Disconnect it's signals as well.
}

void Editor::_add_movement_script() {
  std::cout << "Adding Movement Script to Player.\n";

  auto *script = selected_script_ptr();
  if (script == nullptr) {
    return;
  }

  // TODO???Use 'namespace BUILT_IN_BLOCKS' functions.
  // as the blocks will be spawned in the editor.
  // As of now, we have a bool to indicate if we want to add a movement
  // script,which will be write movement script when exporting the code. We
  // have no way to change the params of the movement script in the editor at
  // the moment with this hacky way.

  auto *sprite = selected_sprite_ptr();
  if (sprite == nullptr) {
    return;
  }

  sprite->add_movement_script = true;

  std::cout << "[Done]Adding Movement Script to Player.\n";
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

sf::Texture *Editor::load_texture(const std::string &texture_file) {
  std::shared_ptr<sf::Texture> texture(new sf::Texture());
  ERR_CRASH_IF(!texture.get()->loadFromFile(texture_file),
               "Error while loading texture.");
  texture.get()->setSmooth(true);
  textures.emplace_back(texture);

  return textures.back().get();
}

std::string Editor::_position_to_string(sf::Vector2f pos) {
  return "Position: X: " + std::to_string(int(pos.x)) +
         " Y: " + std::to_string(int(pos.y));
}

void Editor::_create_new_sprite() {
  auto new_sprite_name = new_sprite_name_input.get_text();
  add_new_sprite(new_sprite_name);
}

void Editor::add_block_to_script(Block b) {
  if (script_editor.script == nullptr) {
    return;
  }

  script_editor.script->blocks.push_back(b);
}

void Editor::_update_sprite_texure() {
  auto *selected_sprite = selected_sprite_ptr();
  if (selected_sprite == nullptr) {
    return;
  }

  RETURN_IF_STRING_HAS_SPACE(sprite_texture_name.get_text(),
                             "Texture file name should not contain space.")

  selected_sprite->texture = sprite_texture_name.get_text();
  selected_sprite->sprite.setTexture(*load_texture(selected_sprite->texture));

  auto texture_size =
      (sf::Vector2f)selected_sprite->sprite.getTexture()->getSize();
  auto sprite_bounds = sf::IntRect(0, 0, texture_size.x, texture_size.y);

  selected_sprite->sprite.setScale(sf::Vector2f(1.0f, 1.0f));
  selected_sprite->sprite.setTextureRect(sprite_bounds);
  selected_sprite->sprite.setOrigin(texture_size.x / 2.0f,
                                    texture_size.y / 2.0f);

  sprite_texture_name.line_input_active = false;
}

void Editor::_update_sprite_name() {
  auto *selected_sprite = selected_sprite_ptr();
  if (selected_sprite == nullptr) {
    return;
  }

  RETURN_IF_STRING_HAS_SPACE(sprite_name.get_text(),
                             "Sprite name shouldn't contain spaces.")

  selected_sprite->name = sprite_name.get_text();
  // Update the text in the UIButton that toggles this sprite, as well.
  for (const auto &[btn_ref, sprite_id] : btn_id_pairs) {
    if (sprite_id == selected_sprite->id) {
      btn_ref.get()->text.setText(selected_sprite->name);
      break; // There's no reason two buttons are assigned to a single sprite.
    }
  }

  sprite_name.line_input_active = false;
  sprite_name.emit_signal("resized");
}

void Editor::select_sprite_by_id(int id) {
  for (const auto &sprite : user_added_sprites) {
    if (sprite.id == id) {
      currently_selected_sprite_id = id;
      sprite_name.set_text(sprite.name);
      sprite_pos.setText(_position_to_string(sprite.position));
      sprite_texture_name.set_text(sprite.texture);
      sprite_layer_value_input.set_text(std::to_string(sprite.layer));
      _highlight_btn_in_list(sprite.id);
      // _refresh_layout();

      // Undrag if any sprite was being dragged.
      // This is because we use right click to de-drag a block.
      // TODO??Fix that.
      script_editor.reset_dragged_block();
      script_editor.script = selected_script_ptr();
      return;
    }
  }
}

void Editor::_highlight_btn_in_list(const int id) {
  // The selected button only looks like button, all other buttons are 'flat',
  // meaning they just look like a label.
  for (const auto &[btn_ref, target_sprite_id] : btn_id_pairs) {
    btn_ref.get()->is_flat = (target_sprite_id != id);
  }
}

void Editor::add_new_sprite(const std::string &p_name) {
  RETURN_IF_STRING_HAS_SPACE(p_name, "Sprite name shouldn't contain space.")

  for (const auto &sprite : user_added_sprites) {
    if (sprite.name == p_name) {
      SHOW_ERROR_ALERT("[Error] Sprite with given name Already Exits.\n");
      return;
    }
  }

  int new_working_id = _total_sprites_added;

  std::shared_ptr<UIButton> btn(new UIButton(p_name));
  btn.get()->is_flat = true;

  btn.get()->clicked_callback = [new_working_id, this]() {
    select_sprite_by_id(new_working_id);
  };

  BtnIDPair pair;
  pair.first = std::move(btn);
  pair.second = new_working_id;

  btn_id_pairs.push_back(pair);

  user_added_sprites_list_vbox.add_child(*pair.first);

  EditorSprite e_spr;
  e_spr.id = new_working_id;
  e_spr.layer = _selected_sprite_layer() + 1;
  e_spr.name = p_name;
  e_spr.position =
      sf::Vector2f(200.0f + (new_working_id * 200), window.getSize().y / 2.0f);
  e_spr.texture = (new_working_id % 2 == 0) ? "cat.png" : "fish.png";
  e_spr.visibility = true;

  sf::Sprite spr;
  spr.setTexture(*load_texture(e_spr.texture));
  sf::FloatRect textureSize = spr.getGlobalBounds();
  spr.setOrigin(textureSize.width / 2.0f, textureSize.height / 2.0f);
  spr.setPosition(e_spr.position);
  e_spr.sprite = spr;

  user_added_sprites.push_back(e_spr);

  std::shared_ptr<Script> script(new Script());
  script.get()->attached_to_sprite_id = new_working_id;
  scripts.push_back(script);

  _total_sprites_added++;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

void Editor::_refresh_layout() {
  user_added_sprites_list_parent.reposition_children();
  editor_inspector.reposition_children();
}

Script *Editor::selected_script_ptr() {
  for (auto &script : scripts) {
    if (script.get()->attached_to_sprite_id == currently_selected_sprite_id) {
      return script.get();
    }
  }

  return nullptr;
}

EditorSprite *Editor::selected_sprite_ptr() {
  if (currently_selected_sprite_id == -1) {
    return nullptr;
  }

  for (auto &sprite : user_added_sprites) {
    if (sprite.id == currently_selected_sprite_id) {
      return &sprite;
    }
  }

  return nullptr;
}

Script *Editor::get_script_attached_to_editor_sprite(EditorSprite *sprite) {
  int sprite_id = sprite->id;
  for (auto &script : scripts) {
    if (script.get()->attached_to_sprite_id == sprite_id) {
      return script.get();
    }
  }

  return nullptr;
}

void Editor::toggle_tab_bar_folding() {
  auto &render_status = built_in_blocks_tab_bar.render_status;
  if (render_status == TabBarStatus::SHOW_ALL) {
    // Gray color,to indicate the button is disabled.
    blocks_tab_bar_collapse_btn.button_fill_color = sf::Color(200, 200, 200);
    render_status = TabBarStatus::SHOW_ONLY_TITLE;
  } else {
    blocks_tab_bar_collapse_btn.button_fill_color = sf::Color::Green;
    render_status = TabBarStatus::SHOW_ALL;
  }
}

void Editor::handle_inputs(sf::Event event) {

  user_added_sprites_list_parent.handle_inputs(event);
  editor_inspector.handle_inputs(event);
  script_editor.handle_inputs(event);

  if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::Slash) {
      toggle_tab_bar_folding();
    } else if (event.key.code == sf::Keyboard::LAlt) {
      _build_and_run();
    }
  } else if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Button::XButton1) {
      // The bottom button on left side of a gaming mouse.
      toggle_tab_bar_folding();
    }
  }

  built_in_blocks_tab_bar.handle_inputs(event);
  blocks_tab_bar_collapse_btn.handle_inputs(event);
  build_and_run_btn.handle_inputs(event);
}

void Editor::_render_ui() {
  // TODO ??? move refreshing to where needed.
  _refresh_layout();
  user_added_sprites_list_parent.Render();
  editor_inspector.Render();
  build_and_run_btn.Render();
}

std::vector<const EditorSprite *> Editor::get_sprites_sorted_by_layers() const {
  std::vector<int> layers;
  layers.reserve(user_added_sprites.size());

  for (const auto &sprite : user_added_sprites) {
    layers.push_back(sprite.layer);
  }

  // Sort Layers.
  std::sort(layers.begin(), layers.end());
  // Remove duplicate layers.
  layers.erase(std::unique(layers.begin(), layers.end()), layers.end());

  std::vector<const EditorSprite *> sorted_sprites;
  sorted_sprites.reserve(user_added_sprites.size());

  for (auto layer : layers) {
    // Add all the sprites that are in this layer.
    for (auto &sprite : user_added_sprites) {
      if (sprite.layer == layer) {
        sorted_sprites.push_back(&sprite);
      }
    }
  }

  return sorted_sprites;
}

void Editor::_render_bounding_box_over_selected_sprite() {
  // This could be done in _render_sprites() below.
  auto *target_object = selected_sprite_ptr();
  if (target_object == nullptr) {
    return;
  }

  auto target_sprite = &target_object->sprite;
  const auto bounds = target_sprite->getGlobalBounds();

  sf::RectangleShape box;
  box.setPosition({bounds.left, bounds.top});
  box.setSize({bounds.width, bounds.height});
  box.setFillColor(sf::Color::Transparent);
  box.setOutlineThickness(2.0f);
  box.setOutlineColor(sf::Color::Black);
  window.draw(box);
}

void Editor::_render_sprites() {
  /*
  for (const auto &sprite : user_added_sprites) {
    window.draw(sprite.sprite);
  }
  */

  // Drawing Sprites Sorted by their layers.
  auto sorted_sprites = get_sprites_sorted_by_layers();
  for (auto &sprite : sorted_sprites) {
    window.draw(sprite->sprite);
  }
}

void Editor::_process_2D_gizmo() {
  auto *target_object = selected_sprite_ptr();
  if (target_object == nullptr) {
    return;
  }

  auto target_sprite = &target_object->sprite;

  gizmo_2D.setTargetSprite(target_sprite);
  // Since that's a vector, the pointer to first element changes on vector
  // resize. So we update every frame.
  // TODO : Find Better way.
  gizmo_2D.Render();

  auto new_pos = target_sprite->getPosition();
  sprite_pos.setText(_position_to_string(new_pos));

  // Setting the position again assuming the position was changed by the
  // gizmo.
  target_object->position = new_pos;
}

void Editor::Render() {
  // TODO???
  // if (sprite_visible) {
  //    ......
  // }
  _render_bounding_box_over_selected_sprite();
  _render_sprites();
  _render_ui();
  _process_2D_gizmo();
  _render_tab();
  script_editor.Render();
}

void Editor::spawn_and_bind_editor_blocks() {

  auto SPAWN_EDITOR_BLOCK_AND_BIND = [&](const block_generator_fn_ptr &fn_ptr) {
    std::cout << "Spawn And Bind.\n";

    // Create a block with the fn_ptr.
    // The fn_ptr takes a block, and adds all the required children.
    Block block;
    fn_ptr(&block);

    // Add the block to the editor.
    editor_blocks.push_back(block);

    // Get the string that uniquely identifies the block and bind that string
    // with the function that generated it in the first place.
    bound_blocks.bind_function(block.function_identifier, fn_ptr);

    std::cout << "Function Identifier: " << block.function_identifier << "\n";
    std::cout << "[Done] Spawn And Bind.\n";
  };
  // The advantage of this lambda is we don't need to manually register the
  // functions. In this way, we create a block for the editor and use that
  // oppurtunity to register it as well.

  editor_blocks.reserve(25);

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

  std::cout << "\n[Done]Creating Editor Blocks:\n\n";
}

void Editor::_spawn_block_at_mouse_pos(const Block &block) {
  std::cout << "User Adding a Block.\n";

  block_generator_fn_ptr fn_ptr =
      bound_blocks.get_bound_block_gen_fn(block.function_identifier).value();

  Block new_block;
  fn_ptr(&new_block);
  new_block.set_position((sf::Vector2f)mouse_position);
  new_block.dragging = true;
  // blocks.push_back(new_block);
  add_block_to_script(new_block);

  // Hide the scripts tab, so we make space for new blocks to
  // spawn. Totally not needed, but as of now, newly spawned
  // blocks render below the tab bar. So, this is kinda hack.
  // toggle_tab_bar_folding();

  std::cout << "[Done]User Adding a Block.\n\n";
}

void Editor::_render_tab() {
  built_in_blocks_tab_bar.Render();
  blocks_tab_bar_collapse_btn.Render();

  if (built_in_blocks_tab_bar.render_status == TabBarStatus::SHOW_ONLY_TITLE) {
    return;
  }

  sf::Vector2f block_in_tabs_draw_position =
      built_in_blocks_tab_bar.get_visible_tab_position() +
      sf::Vector2f(50, 50 + built_in_blocks_tab_bar.get_scroll_value() * 20.0f);

  int currently_selected_tab = built_in_blocks_tab_bar.currently_selected_tab;

  bool is_any_block_being_dragged = script_editor.is_any_block_dragging();
  bool can_spawn_editor_block = !is_any_block_being_dragged &&
                                sf::Mouse::isButtonPressed(sf::Mouse::Left);

  // These are editor blocks which are for spawning new blocks.
  for (auto &block : editor_blocks) {
    if (block.TabItBelongsToName != currently_selected_tab) {
      continue;
    }

    block.set_position(block_in_tabs_draw_position);

    // Scrolling above the tab.
    if (block_in_tabs_draw_position.y >=
        built_in_blocks_tab_bar.get_visible_tab_position().y) {
      block.Render();

      // Select the blocks only that are visible.
      if (can_spawn_editor_block) {
        // Spawn New Block.
        if (block.is_mouse_over()) {
          _spawn_block_at_mouse_pos(block);
          can_spawn_editor_block = false;
        }
      }
    }

    float height_of_cur_block = block.block_full_size.y;
    block_in_tabs_draw_position.y += height_of_cur_block + 20;
  }
}

void Editor::_build_and_run() {
  CodeGenerator CodeGen(*this);
  CodeGen.generate_code();
  std::system("GeneratedOutput\\build_and_execute.bat");
}