#include "editor.hpp"

sf::Texture *Editor::load_texture(const std::string &texture_file) {
  std::shared_ptr<sf::Texture> texture(new sf::Texture());
  ERR_FAIL_COND_CRASH(!texture.get()->loadFromFile(texture_file),
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

EditorSprite *Editor::_currently_selected_sprite_ptr() {
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

void Editor::_update_sprite_texure() {
  auto *selected_sprite = _currently_selected_sprite_ptr();
  if (selected_sprite == nullptr) {
    return;
  }

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
  auto *selected_sprite = _currently_selected_sprite_ptr();
  if (selected_sprite == nullptr) {
    return;
  }

  selected_sprite->name = sprite_name.get_text();
  if (selected_sprite->ui_btn_ref != nullptr) {
    // Update the label of the button, which points to the currently
    // selected sprite.
    selected_sprite->ui_btn_ref->text.setText(selected_sprite->name);
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
      _highlight_selected_btn_in_list(sprite.ui_btn_ref);
      _refresh_layout();
      return;
    }
  }
}

void Editor::_highlight_selected_btn_in_list(const UIButton *btn_to_highlight) {
  for (auto &btn : user_added_sprite_ptrs) {
    // Set flat to everything.
    btn.get()->is_flat = true;
    if (btn.get() == btn_to_highlight) {
      // Toggle back the only selected btn
      btn.get()->is_flat = false;
    }
  }
}

void Editor::add_new_sprite(const std::string &p_name) {
  for (const auto &sprite : user_added_sprites) {
    if (sprite.name == p_name) {
      std::cout << "[Error] Sprite with given name Already Exits.\n";
      return;
    }
  }

  int new_working_id = _total_sprites_added;

  std::shared_ptr<UIButton> btn(new UIButton(p_name));
  btn.get()->is_flat = true;

  btn.get()->clicked_callback = [new_working_id, this]() {
    std::cout << "Assigned ID: " << new_working_id << " for callback. \n";
    select_sprite_by_id(new_working_id);
  };

  // Adding the shared_ptr to a vector, so that it's lifetime is untill
  // the program ends and is freed when the program ends.
  user_added_sprite_ptrs.emplace_back(btn);
  user_added_sprites_list.add_child(*btn);

  EditorSprite e_spr;
  e_spr.id = new_working_id;
  e_spr.name = p_name;
  e_spr.position =
      sf::Vector2f(200.0f + (new_working_id * 200), window.getSize().y / 2.0f);
  e_spr.texture = _total_sprites_added ? "cat.png" : "fish.png";
  e_spr.ui_btn_ref = btn.get();
  e_spr.visibility = true;

  sf::Sprite spr;
  spr.setTexture(*load_texture(e_spr.texture));
  sf::FloatRect textureSize = spr.getGlobalBounds();
  spr.setOrigin(textureSize.width / 2.0f, textureSize.height / 2.0f);
  spr.setPosition(e_spr.position);
  e_spr.sprite = spr;

  user_added_sprites.push_back(e_spr);

  _total_sprites_added++;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

void Editor::_refresh_layout() {
  user_added_sprites_list.reposition_children();
  editor_inspector.reposition_children();
}

void Editor::handle_inputs(sf::Event event) {
  user_added_sprites_list.handle_inputs(event);
  editor_inspector.handle_inputs(event);
}

void Editor::_render_ui() {
  // TODO ??? move refreshing to where needed.
  _refresh_layout();
  user_added_sprites_list.Render();
  editor_inspector.Render();
}

void Editor::_render_sprites() {
  for (const auto &sprite : user_added_sprites) {
    window.draw(sprite.sprite);
  }
}

void Editor::_process_2D_gizmo() {
  auto *target_object = _currently_selected_sprite_ptr();
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
  _render_ui();
  _render_sprites();
  _process_2D_gizmo();
}
