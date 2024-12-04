#include "Editor.hpp"
#include "../FileSystemUtils.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../Nodes/RectangleShapeNode.hpp"
#include "../Nodes/SpriteNode.hpp"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <thread>

int Editor::_selected_sprite_layer() {
  int layer_value = std::stoi(sprite_layer_value_input.text.get_text());
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

  // Sprites layers have changed, so we need to update the cache as well.
  update_sorted_sprites_cache();
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
  auto *script = selected_script_ptr();
  if (script == nullptr) {
    return;
  }

  auto *sprite = selected_sprite_ptr();
  if (sprite == nullptr) {
    return;
  }

  // As of now, we have a bool to indicate if we want to add a movement
  // script, which will write movement script when exporting the code. We
  // have no way to change the params of the movement script in the editor at
  // the moment with this hacky way.
  sprite->add_movement_script = true;

  ALERT("Added Movement Script.");
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

std::optional<sf::Texture *>
Editor::load_texture(const std::string &texture_file) {
  std::shared_ptr<sf::Texture> texture(new sf::Texture());
  if (!texture.get()->loadFromFile(texture_file)) {
    SHOW_ERROR_ALERT("Error while loading texture.");
    return std::nullopt;
  }

  texture.get()->setSmooth(true);
  textures.emplace_back(texture);

  return textures.back().get();
}

std::string Editor::_position_to_string(sf::Vector2f pos) {
  return "Position: X: " + std::to_string(static_cast<int>(pos.x)) +
         " Y: " + std::to_string(static_cast<int>(pos.y));
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

  /*
    RETURN_IF_STRING_HAS_SPACE(sprite_texture_name.get_text(),
                               "Texture file name should not contain space.")
  */

  auto texture_ref = load_texture(sprite_texture_name.get_text());
  if (!texture_ref) {
    return;
  }

  selected_sprite->texture = sprite_texture_name.get_text();

  // Don't show the full path of the image, as the load button next to its
  // overflows. So, we just display the image file name.
  sprite_texture_name.set_text(FileSystemUtils::_image_file_name_from_full_path(
      sprite_texture_name.get_text()));

#define OBJECT_IS(T)                                                           \
  auto casted = dynamic_cast<T *>(selected_sprite->node.get())
  if (OBJECT_IS(SpriteNode)) {
    auto &sprite_ref = casted->get_shape();

    sprite_ref.setTexture(*texture_ref.value());

    auto texture_size = (sf::Vector2f)sprite_ref.getTexture()->getSize();
    auto sprite_bounds = sf::IntRect(0, 0, texture_size.x, texture_size.y);

    sprite_ref.setScale(sf::Vector2f(1.0f, 1.0f));
    sprite_ref.setTextureRect(sprite_bounds);
    sprite_ref.setOrigin(texture_size.x / 2.0f, texture_size.y / 2.0f);
  }
#undef OBJECT_IS

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

  // Update the text in the UITreeView item that toggles this sprite, as well.
  auto index = _get_index_of_btn_related_to_editor_sprite(selected_sprite->id);
  if (index) {
    sprite_list_ui_tree.update_item_name(index.value(), selected_sprite->name);
  }

  sprite_name.line_input_active = false;
  sprite_name.emit_signal("resized");
}

void Editor::select_sprite_by_id(int id) {
  for (auto &sprite : user_added_sprites) {
    if (sprite.id == id) {
      currently_selected_sprite_id = id;
      Cache.selected_sprite_ptr = &sprite;
      sprite_name.set_text(sprite.name);
      sprite_pos.set_text(_position_to_string(sprite.position));
      sprite_texture_name.set_text(
          FileSystemUtils::_image_file_name_from_full_path(sprite.texture));
      sprite_layer_value_input.set_text(std::to_string(sprite.layer));
      _highlight_btn_in_list(sprite.id);
      // _refresh_layout();

      // Undrag if any sprite was being dragged.
      // This is because we use right click to de-drag a block.
      // TODO??Fix that.
      script_editor.reset_dragged_block();
      script_editor.script = selected_script_ptr();

      // When object selection changes, we need to fetch new properties.
      // So that's why we call this function here.
      properties_list_ui.build_initial_property_list_ui(sprite.node.get());
      return;
    }
  }
}

std::optional<int>
Editor::_get_index_of_btn_related_to_editor_sprite(int p_sprite_id) {
  // Find the id of UITree item that matches the provided sprite id.
  int idx = -1;
  for (const auto &[item_id, sprite_id] : tree_item_sprite_index_pairs) {
    if (sprite_id == p_sprite_id) {
      idx = item_id;
      break;
    }
  }

  if (idx == -1) {
    return {};
  }

  // Find the actual index in the vector that contains the given list item.
  auto id = sprite_list_ui_tree.index_of_item_with_id(idx);
  if (id.has_value()) {
    return id.value();
  }

  // Should never happen :(.
  return {};
}

void Editor::_highlight_btn_in_list(const int id) {
  auto index = _get_index_of_btn_related_to_editor_sprite(id);
  if (index) {
    sprite_list_ui_tree.select_item_by_index(index.value());
  }
}

std::string Editor::get_unique_sprite_name_from(const std::string &name) {
  // Search the name in the map.
  auto it = added_sprite_names.find(name);
  auto found = it != added_sprite_names.end();

  if (found) {
    // That name already exists.
    auto new_count = it->second + 1;
    added_sprite_names[name] = new_count;

    auto new_name = name + "_" + std::to_string(new_count);
    return get_unique_sprite_name_from(new_name);
  } else {
    // No names with that till now. So, register that name.
    added_sprite_names[name] = 0;

    return name;
  }
}

void Editor::add_new_sprite(const std::string &p_sprite_name) {
  RETURN_IF_STRING_HAS_SPACE(p_sprite_name,
                             "Sprite name shouldn't contain space.")

  auto p_name = get_unique_sprite_name_from(p_sprite_name);

  int new_working_id = _total_sprites_added;

  auto select_sprite_func = [new_working_id, this]() {
    select_sprite_by_id(new_working_id);
  };

  auto list_item_id = sprite_list_ui_tree.add_item(p_name, select_sprite_func);

  ListItemSpritePair pair;
  pair.list_item_id = list_item_id;
  pair.sprite_id = new_working_id;
  tree_item_sprite_index_pairs.push_back(pair);

  EditorSprite e_spr;
  e_spr.id = new_working_id;
  e_spr.layer = _selected_sprite_layer() + 1;
  e_spr.name = p_name;
  e_spr.position =
      sf::Vector2f(200.0f + (new_working_id * 200), window.getSize().y / 2.0f);
  e_spr.texture = (new_working_id % 2 == 0) ? "cat.png" : "fish.png";
  e_spr.visibility = true;

  auto texture_ref = load_texture(e_spr.texture);
  if (!texture_ref) {
    // Should never happen, because we always have "cat" & "fish" textures.
    return;
  }

  std::shared_ptr<Node> n_node;

  const auto selection = add_node_type_drop_down.get_text();

  if (selection == "CircleShape") {
    sf::CircleShape shape;
    shape.setPosition(e_spr.position);
    shape.setFillColor(sf::Color::Yellow);
    shape.setRadius(100.0F);

    n_node = std::make_shared<CircleShapeNode>(shape);
  } else if (selection == "RectangleShape") {
    sf::RectangleShape shape;
    shape.setPosition(e_spr.position);
    shape.setSize(sf::Vector2f(225, 150));
    shape.setFillColor(sf::Color::Yellow);

    n_node = std::make_shared<RectangleShapeNode>(shape);
  } else if (selection == "Sprite" || true) {
    sf::Sprite spr;
    spr.setTexture(*texture_ref.value());
    sf::FloatRect textureSize = spr.getGlobalBounds();
    spr.setOrigin(textureSize.width / 2.0f, textureSize.height / 2.0f);
    spr.setPosition(e_spr.position);

    n_node = std::make_shared<SpriteNode>(spr);
  }

  e_spr.node = std::move(n_node);

  user_added_sprites.push_back(e_spr);
  update_sorted_sprites_cache();

  std::shared_ptr<Script> script(new Script());
  script.get()->attached_to_sprite_id = new_working_id;
  scripts.push_back(script);

  _total_sprites_added++;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

void Editor::_refresh_layout() { editor_inspector.reposition_children(); }

Script *Editor::selected_script_ptr() {
  for (auto &script : scripts) {
    if (script.get()->attached_to_sprite_id == currently_selected_sprite_id) {
      return script.get();
    }
  }

  return nullptr;
}

EditorSprite *Editor::_selected_sprite_ptr() {
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
    blocks_tab_bar_collapse_btn.default_fill_color = sf::Color(200, 200, 200);
    render_status = TabBarStatus::SHOW_ONLY_TITLE;
  } else {
    blocks_tab_bar_collapse_btn.default_fill_color = sf::Color::Green;
    render_status = TabBarStatus::SHOW_ALL;
  }
}

void Editor::_handle_2D_world_inputs(sf::Event event) {
  if (isMouseOverRect(world)) {
    if (event.type == sf::Event::MouseWheelMoved) {
      view.zoom(1.0f - event.mouseWheel.delta / 10.0f);
    } else if (event.type == sf::Event::KeyReleased) {
      if (event.key.code == sf::Keyboard::H) {
        is_panning = !is_panning;
        if (is_panning) {
          setCursor(sf::Cursor::SizeAll);
        } else {
          setCursor(sf::Cursor::Arrow);
        }
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      view.move(5.f, 0.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      view.move(-5.f, 0.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      view.move(0.f, 5.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      view.move(0.f, -5.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
      view.zoom(0.99f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
      view.zoom(1.01f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
      gizmo_2D.current_gizmo_state = GIZMO_SELECT_STATE::CENTER;

      auto mouse_world_pos = window.mapPixelToCoords(
          static_cast<sf::Vector2i>(get_mouse_position()), view);

      gizmo_2D.offset =
          mouse_world_pos - selected_sprite_ptr()->get_node()->getPosition();
    }

    if (gizmo_2D.is_gizmo_selected()) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        gizmo_2D.current_gizmo_state = GIZMO_SELECT_STATE::CENTER;
        gizmo_2D.offset = sf::Vector2f();
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        gizmo_2D.current_gizmo_state = GIZMO_SELECT_STATE::X;

        auto mouse_world_pos = window.mapPixelToCoords(
            static_cast<sf::Vector2i>(get_mouse_position()), view);
        gizmo_2D.offset.x = mouse_world_pos.x -
                            selected_sprite_ptr()->get_node()->getPosition().x;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
        gizmo_2D.current_gizmo_state = GIZMO_SELECT_STATE::Y;

        auto mouse_world_pos = window.mapPixelToCoords(
            static_cast<sf::Vector2i>(get_mouse_position()), view);
        gizmo_2D.offset.y = mouse_world_pos.y -
                            selected_sprite_ptr()->get_node()->getPosition().y;
      }
    }

    if (is_panning) {
      auto delta = old_mouse_pos - new_mouse_pos;
      view.move(delta);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      _pick_sprite();
    }
  }
}

void Editor::_handle_sprite_list_inputs(sf::Event event) {
  new_sprite_hbox.handle_inputs(event);
  sprite_list_ui_tree.handle_inputs(event);
}

void Editor::handle_inputs(sf::Event event) {
  old_mouse_pos = new_mouse_pos;
  new_mouse_pos = get_mouse_position();

  // TODO : Add these checks before handling inputs for different editor
  // sections.
  // if (isMouseOverRect(...)) {

  _handle_2D_world_inputs(event);
  _handle_sprite_list_inputs(event);

  info_tab.handle_inputs(event);

  auto selected_info_tab = info_tab.get_currently_selected_tab();
  if (selected_info_tab == 0) {
    editor_inspector.handle_inputs(event);
  } else if (selected_info_tab == 1) {
    properties_list_ui.handle_inputs(event);
  }

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
  add_node_type_drop_down.handle_inputs(event);
}

void Editor::_render_sprite_list_ui() {
  new_sprite_hbox.Render();
  sprite_list_ui_tree.Render();
}

void Editor::_render_ui() {
  // TODO ??? move refreshing to where needed.
  _refresh_layout();
  _render_sprite_list_ui();
  info_tab.Render();

  auto selected_info_tab = info_tab.get_currently_selected_tab();
  if (selected_info_tab == 0) {
    editor_inspector.Render();
  } else if (selected_info_tab == 1) {
    properties_list_ui.Update();
    properties_list_ui.Render();
  }

  build_and_run_btn.Render();
  add_node_type_drop_down.Render();
}

void Editor::update_sorted_sprites_cache() {
  // Adding new sprite invalidates this ptr, so refetch it as well.
  Cache.selected_sprite_ptr = _selected_sprite_ptr();

  Cache.sprites_sorted_by_layers.clear();
  Cache.sprites_sorted_by_layers = get_sprites_sorted_by_layers();

  Cache.sprites_sorted_by_layers_reverse.clear();
  Cache.sprites_sorted_by_layers_reverse = Cache.sprites_sorted_by_layers;

  // Top most sprites need to pick up the inputs first, so we reverse the list.
  std::reverse(Cache.sprites_sorted_by_layers_reverse.begin(),
               Cache.sprites_sorted_by_layers_reverse.end());
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

  auto target_sprite = target_object->get_node();
  const auto bounds = target_sprite->getGlobalBounds();

  sf::RectangleShape box;
  box.setPosition(bounds.getPosition());
  box.setSize(bounds.getSize());
  box.setFillColor(sf::Color::Transparent);
  box.setOutlineThickness(2.0f);
  box.setOutlineColor(sf::Color::Black);
  window.draw(box);
}

void Editor::_render_sprites() {
  /*
  for (const auto &sprite : user_added_sprites) {
    //TODO ?? if(!sprite.visible){continue;}
    window.draw(sprite.sprite);
  }
  */

  // Drawing Sprites Sorted by their layers.
  for (auto &sprite : Cache.sprites_sorted_by_layers) {
    window.draw(*sprite->get_node());
  }
}

void Editor::_on_sprites_translation_update(bool update_translation,
                                            bool update_scale) {
  auto *target_object = selected_sprite_ptr();
  if (target_object == nullptr) {
    return;
  }

  auto target_sprite = target_object->get_node();
  if (update_translation) {
    auto new_pos = target_sprite->getPosition();
    sprite_pos.set_text(_position_to_string(new_pos));

    // Setting the position again as the position was changed by the
    // gizmo.
    target_object->position = new_pos;

    target_object->node.get()->setPosition(new_pos);
  }

  if (update_scale) {
    // TODO ?? Custom Gizmos to change stuffs like circle's radius.

#define OBJECT_IS(T) auto casted = dynamic_cast<T *>(target_object->node.get())
    if (OBJECT_IS(SpriteNode)) {
      casted->get_shape().setScale(target_sprite->getScale());
    } else if (OBJECT_IS(CircleShapeNode)) {
      casted->get_shape().setScale(target_sprite->getScale());
    } else if (OBJECT_IS(RectangleShapeNode)) {
      casted->get_shape().setScale(target_sprite->getScale());
    }
#undef OBJECT_IS
  }
}

void Editor::_process_2D_gizmo() {
  auto *target_object = selected_sprite_ptr();
  if (target_object == nullptr) {
    return;
  }

  gizmo_2D.setTargetEditorSprite(target_object);
  gizmo_2D.Render();
}

void Editor::_pick_sprite() {
  window.setView(view);

  for (auto &sprite : Cache.sprites_sorted_by_layers_reverse) {
    if (isMouseOverSprite(sprite->get_node())) {
      select_sprite_by_id(sprite->id);
      break;
    }
  }

  window.setView(window.getDefaultView());
}

void Editor::Render() {
  // Setup & Draw 2D World.
  auto deltaTime = frameClock.restart();
  auto delta = deltaTime.asSeconds();

  window.draw(world2d_border);
  window.setView(view);
  window.draw(grid_x_axis_line);
  window.draw(grid_y_axis_line);
  _render_sprites();
  _render_bounding_box_over_selected_sprite();
  _process_2D_gizmo();
  window.setView(window.getDefaultView());

  block_visualizer.Update(delta);

  // Render UI on top of all.
  _render_ui();
  _render_block_spawner_tab();

  if (gizmo_2D.is_gizmo_selected()) {
    escape_sprite_dragging_label.Render();
  }

  script_editor.Render();
}

void Editor::spawn_and_bind_editor_blocks() {

  constexpr bool DEBUG_BLOCK_SPAWN_BIND_STATS = false;

  auto SPAWN_EDITOR_BLOCK_AND_BIND = [&](const block_generator_fn_ptr &fn_ptr) {
    if (DEBUG_BLOCK_SPAWN_BIND_STATS) {
      std::cout << "Spawn And Bind.\n";
    }

    // Create a block with the fn_ptr.
    // The fn_ptr takes a block, and adds all the required children.
    Block block;
    fn_ptr(&block);

    // Add the block to the corresponding tab in the editor.
    editor_blocks[block.TabItBelongsToName].push_back(block);

    // Get the string that uniquely identifies the block and bind that string
    // with the function that generated it in the first place.
    bound_blocks.bind_function(block.function_identifier, fn_ptr);

    if (DEBUG_BLOCK_SPAWN_BIND_STATS) {
      std::cout << "Function Identifier: " << block.function_identifier << "\n";
      std::cout << "[Done] Spawn And Bind.\n";
    }
  };
  // The advantage of this lambda is we don't need to manually register the
  // functions. In this way, we create a block for the editor and use that
  // oppurtunity to register it as well.

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_program_started);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_sprite_clicked);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_forever);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_key_pressed);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_sprite_touching);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_if);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_alternate_between);

  // There are two ways of adding character controller for a script.
  // By Using the editor block.
  // The caveat is that it should be placed in forever block(Recommended).
  // Another method is by using the editors "Add Controller" button, which
  // silently writes the controller code during export, but the speed of this
  // movement isn't configurable and defaults to 200.
  SPAWN_EDITOR_BLOCK_AND_BIND(
      BUILT_IN_BLOCKS::block_default_character_controller);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_go_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_move_by_vector2);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_x_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_x_by_in);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_y_by);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_change_y_by_in);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_to_xy);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_glide_point_to_point);

  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_say);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_draw_text);
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
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_normalize_vector2f);
  SPAWN_EDITOR_BLOCK_AND_BIND(BUILT_IN_BLOCKS::block_update_vector2f);

  if (DEBUG_BLOCK_SPAWN_BIND_STATS) {
    std::cout << "\n[Done]Creating Editor Blocks:\n\n";
    std::cout << "TAB_CONTROL count : "
              << editor_blocks[BLOCKS_TAB_NAME::TAB_CONTROL].size() << "\n";
    std::cout << "TAB_DRAW_PRIMITIVES count : "
              << editor_blocks[BLOCKS_TAB_NAME::TAB_DRAW_PRIMITIVES].size()
              << "\n";
    std::cout << "TAB_MOTION count : "
              << editor_blocks[BLOCKS_TAB_NAME::TAB_MOTION].size() << "\n";
    std::cout << "TAB_VARIABLES count : "
              << editor_blocks[BLOCKS_TAB_NAME::TAB_VARIABLES].size() << "\n";
  }
}

void Editor::_spawn_block_at_mouse_pos(const Block &block) {
  // std::cout << "User Adding a Block.\n";

  // Set mouse position to the middle of the script editor & spawn new blocks
  // there.
  auto script_editor_world = script_editor.get_world();
  auto block_spawn_location = sf::Vector2f(
      script_editor_world.left + (script_editor_world.width / 2.0F),
      script_editor_world.top + (script_editor_world.height / 2.0F));
  sf::Mouse::setPosition(static_cast<sf::Vector2i>(block_spawn_location));

  block_generator_fn_ptr fn_ptr =
      bound_blocks.get_bound_block_gen_fn(block.function_identifier).value();

  Block new_block;
  fn_ptr(&new_block);
  new_block.set_position(block_spawn_location);
  new_block.dragging = true;
  // blocks.push_back(new_block);
  add_block_to_script(new_block);

  // Hide the scripts tab, so we make space for new blocks to
  // spawn. Totally not needed, but as of now, newly spawned
  // blocks render below the tab bar. So, this is kinda hack.
  // toggle_tab_bar_folding();

  // std::cout << "[Done]User Adding a Block.\n\n";
}

void Editor::_render_block_spawner_tab() {
  built_in_blocks_tab_bar.Render();
  blocks_tab_bar_collapse_btn.Render();

  if (built_in_blocks_tab_bar.render_status == TabBarStatus::SHOW_ONLY_TITLE) {
    return;
  }

  sf::FloatRect tab_world = built_in_blocks_tab_bar.get_tab_body_size();

  bool mouse_over_tab_body = tab_world.contains(get_mouse_position());
  bool tab_body_clicked =
      mouse_over_tab_body && sf::Mouse::isButtonPressed(sf::Mouse::Left);

  sf::View tab_view({0, 0, tab_world.width, tab_world.height});
  auto win_size = window.getSize();

  tab_view.setViewport({tab_world.left / win_size.x, tab_world.top / win_size.y,
                        tab_world.width / win_size.x,
                        tab_world.height / win_size.y});

  window.setView(tab_view);

  built_in_blocks_tab_bar.can_scroll_up = true;
  built_in_blocks_tab_bar.can_scroll_down = true;

  sf::Vector2f draw_position = sf::Vector2f(
      50, 10 + built_in_blocks_tab_bar.get_current_tab_body_scroll() * 30.0f);

  // This is the position of the first block.
  // Initially, the y is 10, if the first block has gone above this, then we
  // shouldn't allow to scroll down.
  if (draw_position.y > 10) {
    // This will be picked in next frame & mouse scroll isn't registered by the
    // input handler.
    built_in_blocks_tab_bar.can_scroll_down = false;
  }

  auto currently_selected_tab = static_cast<BLOCKS_TAB_NAME>(
      built_in_blocks_tab_bar.get_currently_selected_tab());

  bool is_any_block_being_dragged = script_editor.is_any_block_dragging();
  bool can_spawn_editor_block = !is_any_block_being_dragged && tab_body_clicked;

  // These are editor blocks which are for spawning new blocks.
  for (auto &block : editor_blocks[currently_selected_tab]) {
    block.set_position(draw_position);
    block.Render();

    // Even if the little bit of top of the block goes above the tab bar,
    // then make it unselectable, as the mouse clicks in that area is for the
    // actual buttons that are in that area.
    if (draw_position.y >= 0.0F) {
      if (can_spawn_editor_block) {
        // Spawn New Block.
        if (block.is_mouse_over()) {
          _spawn_block_at_mouse_pos(block);
          can_spawn_editor_block = false;
        }
      }
    }

    float height_of_cur_block = block.block_full_size.y;
    draw_position.y += height_of_cur_block + 20;

    if (draw_position.y > tab_world.top + tab_world.height) {
      break;
    }
  }

  // The bottom part of the last block has entered our view.
  // Now, we shouldn't allow to scroll up.
  if (draw_position.y < tab_world.top + tab_world.height) {
    built_in_blocks_tab_bar.can_scroll_up = false;
  }

  window.setView(window.getDefaultView());
}

void Editor::_build_and_run() {
  CodeGenerator CodeGen(*this);
  CodeGen.generate_code();

  std::thread build_thread([]() {
    std::system("GeneratedOutput\\build_and_execute.bat");
  });

  // Detach the thread so it runs independently.
  build_thread.detach();
}