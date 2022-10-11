#include "Script.hpp"

void ScriptEditor::handle_inputs(sf::Event event) {
  if (script == nullptr) {
    return;
  }

  middle_click = false;

  for (auto &block : script->blocks) {
    block._process_events(event);
  }

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Middle) {
    middle_click = true;
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

void ScriptEditor::Update() {
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

void ScriptEditor::Render() {
  if (script == nullptr) {
    return;
  }

  Update();

  for (auto &block : script->blocks) {
    block.Render();
  }
}
