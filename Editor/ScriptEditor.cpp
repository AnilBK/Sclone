#include "ScriptEditor.hpp"

void ScriptEditor::setRect(sf::FloatRect rect) {
  world = rect;
  view = sf::View({0, 0, rect.width, rect.height});
  // sf::View({0, 0, 1200, 800});

  auto w_size = window.getSize();

  view.setViewport({world.left / w_size.x, world.top / w_size.y,
                    world.width / w_size.x, world.height / w_size.y});

  border.setPosition(world.getPosition());
  border.setSize(world.getSize());
  border.setFillColor(sf::Color(153, 195, 180));
  border.setOutlineColor(sf::Color(71, 71, 71));
  border.setOutlineThickness(4.0f);
}

void ScriptEditor::handle_panning(sf::Event event) {
  old_mouse_pos = new_mouse_pos;
  new_mouse_pos = get_mouse_position();

  if (event.type == sf::Event::MouseWheelMoved) {
    view.zoom(1.0f - event.mouseWheel.delta / 10.0f);
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
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
    auto delta = old_mouse_pos - new_mouse_pos;
    view.move(delta);
  }
}

void ScriptEditor::handle_inputs(sf::Event event) {
  if (isMouseOverRect(world)) {
    handle_panning(event);

    window.setView(view);

    if (script != nullptr) {
      static bool lock_click = false;
      bool left_clicked = false;

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && !lock_click) {
          left_clicked = true;
        }
      }

      // Mouse button Released now.
      if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          lock_click = false;
          // Unlock when the button has been released.
        }
      }

      bool left_click_consumed = false;

      for (auto &block : script->blocks) {
        block.handle_inputs(event);

        if (left_clicked) {
          if (block.dragging && !block.is_attached_to_anything) {
            // 'block.is_attached_to_anything' check makes sure, only the top of
            // the dragged block is checked for snap hinting. With this the
            // blocks attached to the currently dragged block aren't checked for
            // snap hinting.

            SNAP_HINT_INFO snap_data = get_block_to_snap_on(&block);
            if (snap_data.can_snap) {
              Block *block_to_snap_to = snap_data.block_to_snap_to;

              if (snap_data.snap_direction == SNAP_DIRECTION::TOP) {
                Block *parent = get_block_it_is_attached_to(block_to_snap_to);
                if (parent != nullptr) {
                  // It is already attached to some block.
                  // So CurrentlyDraggedBlock will be attached to that parent.
                  block.is_attached_to_anything = false;
                  parent->attach_block_next(&block);
                }

                // And that parent's previously attached block will be
                // attached to currently dragged block. Meaning we insert the
                // currently dragged block between them two.
                block.dragging = false;
                block_to_snap_to->is_attached_to_anything = false;
                block.attach_block_next(block_to_snap_to);
                left_click_consumed = true;
              } else if (snap_data.snap_direction == SNAP_DIRECTION::BOTTOM) {
                block.dragging = false;
                block_to_snap_to->attach_block_next(&block);
                left_click_consumed = true;
              } else if (snap_data.snap_direction == SNAP_DIRECTION::INSIDE) {
                block.dragging = false;
                BlockAttachNode *target = snap_data.attach_block_inside_node;
                target->attached_block = &block;
                target->attached_block->dragging = false;
                left_click_consumed = true;
              }
            }
          }
        }

        // Send left click signal to every blocks.
        // If any of the blocks accepts the left click then the left click is
        // consumed, and that left click isn't to be accepted by any other
        // block.

        if (!left_click_consumed && left_clicked) {
          left_click_consumed = block.left_clicked(event);
        }
      }
    }

    window.setView(window.getDefaultView());
  } else {
    if (script != nullptr) {
      for (auto &block : script->blocks) {
        block.background_inputs(event);
      }
    }
  }
}

Block *ScriptEditor::get_block_it_is_attached_to(Block *block_to_test) {
  Block *parent = nullptr;

  for (auto &block : script->blocks) {
    if (block.next_block == nullptr) {
      continue;
    }

    if (block.next_block == block_to_test) {
      parent = &block;
      break;
    }
  }

  return parent;
}

Block *ScriptEditor::get_currently_dragging_block() {
  Block *current_dragging_block_ref = nullptr;

  for (auto &block : script->blocks) {
    if (block.dragging) {
      current_dragging_block_ref = &block;
      break;
    }
  }

  return current_dragging_block_ref;
}

bool ScriptEditor::is_any_block_dragging() {
  if (script == nullptr) {
    return false;
  }

  for (const auto &block : script->blocks) {
    if (block.dragging) {
      return true;
    }
  }

  return false;
}

void ScriptEditor::reset_dragged_block() {
  if (script == nullptr) {
    return;
  }

  for (auto &block : script->blocks) {
    if (block.dragging) {
      block.dragging = false;
    }
  }
}

SNAP_HINT_INFO
ScriptEditor::get_block_to_snap_on(Block *current_dragging_block_ref) {
  SNAP_HINT_INFO snap_data;
  snap_data.can_snap = false;
  snap_data.snap_direction = SNAP_DIRECTION::NONE;
  snap_data.block_to_snap_to = nullptr;
  snap_data.attach_block_inside_node = nullptr;

  for (auto &block : script->blocks) {
    // Can't snap to itself.
    if (current_dragging_block_ref == &block) {
      continue;
    }

    // Control Blocks don't attach to anything.
    if (current_dragging_block_ref->is_control_block()) {
      continue;
    }

    if (block.can_mouse_snap_to_top()) {
      snap_data.can_snap = true;
      snap_data.snap_direction = SNAP_DIRECTION::TOP;
      snap_data.block_to_snap_to = &block;
      break;
    }

    if (block.can_mouse_snap_to_bottom()) {
      snap_data.can_snap = true;
      snap_data.snap_direction = SNAP_DIRECTION::BOTTOM;
      snap_data.block_to_snap_to = &block;
      break;
    }

    auto get_node_to_snap_inside = block.get_node_to_attach_block_inside();
    if (get_node_to_snap_inside.has_value()) {
      snap_data.can_snap = true;
      snap_data.snap_direction = SNAP_DIRECTION::INSIDE;
      snap_data.attach_block_inside_node = get_node_to_snap_inside.value();
      break;
    }
  }

  return snap_data;
}

void ScriptEditor::ShowSnapHints() {
  if (script == nullptr) {
    return;
  }

  Block *current_dragging_block_ref = get_currently_dragging_block();

  bool no_block_is_being_dragged = current_dragging_block_ref == nullptr;
  if (no_block_is_being_dragged) {
    return;
  }

  for (auto &block : script->blocks) {
    if (current_dragging_block_ref == &block) {
      continue;
    }

    // Control Blocks don't attach to anything.
    if (current_dragging_block_ref->is_control_block()) {
      continue;
    }

    block.show_possible_snap_hints();
  }
}

void ScriptEditor::Render() {
  window.draw(border);

  if (script != nullptr) {
    window.setView(view);

    ShowSnapHints();

    for (auto &block : script->blocks) {
      block.Render();
    }

    window.setView(window.getDefaultView());
  }
}
