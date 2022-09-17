#include "Block.hpp"

void Block::_move_attached_blocks(sf::Vector2f p_pos) {
  if (attached_blocks.size() == 0) {
    return;
  }

  // Since positions are already cached.
  // TODO? Are positions cached always??
  // TODO: Maybe create a cache of all positions and use that everywhere.
  // Seems like that is the best thing to do.
  std::vector<sf::Vector2f> attached_blocks_positions;

  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      attached_blocks_positions.push_back(child->_pos +
                                          sf::Vector2f(15.0f, 0.0f));
    }
  }

  for (auto &[index, block_ptr] : attached_blocks) {
    auto block_pos = attached_blocks_positions.at(index);
    block_ptr->set_position(block_pos);
  }
}

void Block::_regenerate_positions() {
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);

  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      pos.x = position.x; // Reset it's x to create a block on the next line.
      pos.y += STARTING_BLOCK_SIZE.y;
      child->_pos = pos;
      pos.x += 15;
      pos.y += 90.0f;
      continue;
    }
    child->_pos = pos;
    pos.x += child->rect_size().x + spacing;
  }
}

void Block::set_position(const sf::Vector2f p_pos) {
  position = p_pos;
  block_rect.setPosition(position);

  // If positions aren't regenrated now, then attached blocks lag by a frame.
  // Because _move_attached_blocks() generates positions using old positions.
  // and new position is only used in Render function to update.
  _regenerate_positions();
  _move_attached_blocks(p_pos);

  if (next_block != nullptr) {
    sf::Vector2f next_block_snap_position(_next_block_snap_rect().left,
                                          _next_block_snap_rect().top);
    next_block->set_position(next_block_snap_position);
  }
}

Block::Block() {
  set_position({0.0f, 0.0f});
  block_rect.setSize(STARTING_BLOCK_SIZE);
  block_rect.setOutlineThickness(2.0f);
  set_block_type(BLOCK_TYPES::INSTRUCTION);
}

void Block::_recalculate_rect() {
  sf::Vector2f pos = position;
  sf::FloatRect merged_rect(pos, STARTING_BLOCK_SIZE);

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Reset it's x to create a block on the next line.
      pos.x = position.x + 15.0f;
      pos.y += merged_rect.height; // STARTING_BLOCK_SIZE.y;
      continue;
    }

    sf::FloatRect current_rect(pos, child->rect_size());
    merged_rect = merge_rects(merged_rect, current_rect);

    pos.x += child->rect_size().x;
    pos.x += spacing;
  }

  // Margins
  // Account for the block position decreased during padding.
  merged_rect.width += padding_left + padding_right;
  merged_rect.height += padding_up + padding_down;

  block_rect.setSize({merged_rect.width, merged_rect.height});
}

bool Block::is_control_block() { return block_type == BLOCK_TYPES::CONTROL; }

sf::FloatRect Block::_previous_block_snap_rect() {
  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() - sf::Vector2f(0.0f, snap_rect_size.y);
  return {snap_rect_position, snap_rect_size};
}

sf::FloatRect Block::_next_block_snap_rect() {
  sf::Vector2f pos{0.0f, STARTING_BLOCK_SIZE.y};

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Reset it's x to create a block on the next line.
      pos.y += 90 + 45; // STARTING_BLOCK_SIZE.y;
      continue;
    }
  }

  // Margins
  // Account for the block position decreased during padding.
  pos.y += padding_up + padding_down;

  auto snap_rect_size = sf::Vector2f(block_rect.getSize().x, 10);
  auto snap_rect_position =
      block_rect.getPosition() + sf::Vector2f(0.0f, pos.y);
  return {snap_rect_position, snap_rect_size};
}

void Block::show_previous_block_snap_hint() {
  auto r = _previous_block_snap_rect();
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width, r.height);

  sf::RectangleShape previous_block_snap_hint;
  previous_block_snap_hint.setPosition(r_pos);
  previous_block_snap_hint.setSize(r_size);

  previous_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(previous_block_snap_hint);
}

void Block::show_next_block_snap_hint() {
  auto r = _next_block_snap_rect();
  auto r_pos = sf::Vector2f(r.left, r.top);
  auto r_size = sf::Vector2f(r.width, r.height);

  sf::RectangleShape next_block_snap_hint;
  next_block_snap_hint.setPosition(r_pos);
  next_block_snap_hint.setSize(r_size);

  next_block_snap_hint.setFillColor(sf::Color::White);
  window.draw(next_block_snap_hint);
}

void Block::process_inside_snap_hints(bool attach_block_requested,
                                      Block *current_dragging_block_ref) {
  // These two Lambdas functions below were implemented in the
  // classe itself, but those gave rise to having a extra virtual function in
  // the base class just for 'BlockAttachNode'.
  auto _attachable_block_snap_hint_rect = [](sf::Vector2f pos) {
    // The attachable block starts from the top left of the vertical 'L' shaped
    // line.
    // What is supposed to be snap highlight.
    auto snap_rect_size = sf::Vector2f(250.0f, 10.0f);
    auto snap_rect_position = pos + sf::Vector2f{15.0f, 0.0f};
    return sf::FloatRect{snap_rect_position, snap_rect_size};
  };

  auto _show_snap_for_attachable_block = [](sf::FloatRect r) {
    // r is _attachable_block_snap_hint_rect.
    auto r_pos = sf::Vector2f(r.left, r.top);
    auto r_size = sf::Vector2f(r.width + 45, r.height);

    sf::RectangleShape block_snap_hint;
    block_snap_hint.setPosition(r_pos);
    block_snap_hint.setSize(r_size);

    block_snap_hint.setFillColor(sf::Color::White);
    window.draw(block_snap_hint);
  };

  int i = 0;
  for (auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      auto r = _attachable_block_snap_hint_rect(child->_pos);
      bool can_attach_inside = r.contains((sf::Vector2f)mouse_position);

      if (can_attach_inside) {
        _show_snap_for_attachable_block(r);
        if (attach_block_requested) {
          // TODO :: Check if the snap place is already occupied.
          // TODO :: current dragging block ref should be pushed back where the
          // snap hint is showing.
          attached_blocks.push_back({i, current_dragging_block_ref});
          current_dragging_block_ref->dragging = false;
          set_position(position); // Refresh for the newly added block.
          return;
        }
      }
      i++;
    }
  }
}

bool Block::can_mouse_snap_to_top() {
  // if (previous != nullptr) {
  // return false;
  // }
  if (is_control_block()) {
    return false;
  }

  return _previous_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

bool Block::can_mouse_snap_to_bottom() {
  if (next_block != nullptr) {
    return false;
  }
  return _next_block_snap_rect().contains((sf::Vector2f)mouse_position);
}

void Block::attach_block_next(Block *p_next_block) {
  if (p_next_block == this) {
    ERR_FAIL_COND_CRASH(false, "Children is same as parent.");
  }

  next_block = p_next_block;
  set_position(position);
  // Set same position again, so it's child's position can be updated too.
}

std::string Block::get_code() {

  std::string code;

  if (output_code_callback) {
    code += output_code_callback(*this);
  }

  if (can_block_snap_inside) {
    code += "{\n";
  }

  if (attached_blocks.size() > 0) {
    // We create a string buffer that is the size of all attachable nodes.
    // since nodes are paired with their respective indices.
    // we get the blocks code and keep it in the respective index of the codes
    // vector, so that it will be easy to iterate over.
    auto n = attached_blocks.size();
    std::vector<std::string> codes(n, "");

    for (std::pair<int, Block *> &a_b : attached_blocks) {
      auto index = a_b.first;
      auto blocks_code = a_b.second->get_code();

      codes.at(index) = blocks_code;
    }

    // Now we have got the generated code in proper order.
    int i = 0;
    for (auto gen_code : codes) {
      code += gen_code;
      if (i + 1 < codes.size()) {
        code += "}else{";
      }
      i++;
    }
  }

  if (can_block_snap_inside) {
    code += "\n}\n";
  }

  if (next_block != nullptr) {
    code += next_block->get_code();
  }

  return code;
}

void Block::RenderRectsBackground() {
  // Debug Code to toggle rendering of the block background.
  static bool render_rect = true;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    render_rect = !render_rect;
  }

  if (!render_rect) {
    return;
  }

  sf::RectangleShape block_bg;
  block_bg.setOutlineThickness(2.0f);
  block_bg.setPosition(position);
  block_bg.setFillColor(block_rect.getFillColor());

  sf::FloatRect merged_rect(position, STARTING_BLOCK_SIZE);

  for (const auto &child : childrens) {
    if (child->type == BLOCK_ATTACH_NODE) {
      // Render everything till now.
      block_bg.setSize({merged_rect.width, merged_rect.height});
      window.draw(block_bg);
      return;
    }

    sf::FloatRect current_rect(child->_pos, child->rect_size());
    merged_rect = merge_rects(merged_rect, current_rect);
  }

  // Margins
  // Account for the block position decreased during padding.
  merged_rect.width += padding_left + padding_right;
  merged_rect.height += padding_up; // + padding_down;

  block_bg.setSize({merged_rect.width, merged_rect.height});
  window.draw(block_bg);
}

// Draw text and all other components.
void Block::RenderComponents() {
  // New Method-Save positions in 'NodeBaseClass' instead.
  _regenerate_positions();
  for (const auto &child : childrens) {
    child->Render(child->_pos);
  }
}

void Block::Render() {
  // Draw the background rect.
  // window.draw(block_rect);
  RenderRectsBackground();
  RenderComponents();
}

std::optional<std::string> Block::get_bound_value(const std::string &query) {
  for (const auto &child : childrens) {
    if (child->bind_string == query) {
      return child->get_text();
    }
  }

  auto unbound_msg_str = "[Debug] String \"" + query + "\" Possibly Unbound.";
  ERR_FAIL_COND_CRASH(false, unbound_msg_str);

  return {};
}

bool Block::_process_left_click_on_children(sf::Event event) {
  // Returns true if any of the child performed 'press' action.
  sf::Vector2f pos = position + sf::Vector2f(padding_left, padding_up);
  // First process child inputs separately.
  for (const auto &child : childrens) {
    if (child->is_mouse_over(pos)) {
      if (child->left_click_action()) {
        return true;
      }
    }

    pos.x += child->rect_size().x;
    pos.x += spacing;
  }

  return false;
}

bool Block::_any_node_already_pressed() {
  for (auto &child : childrens) {
    if (child->pressed) {
      return true;
    }
  }
  return false;
}

void Block::_deselect_all_nodes() {
  for (auto &child : childrens) {
    if (child->pressed) {
      child->pressed = false;
    }
  }
}

void Block::_process_events(sf::Event event) {
  // For some reason, if we do this check on Render()
  // then the value resets after we press right click.
  // Weird.
  if (dragging) {
    set_position((sf::Vector2f)mouse_position);
  }

  bool rect_dirty = false;
  bool any_line_inputs_pressed = false;

  for (auto &child : childrens) {
    if (child->type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD ||
        child->type == NODE_TYPE::DROP_DOWN) {
      child->_process_event(event);
      if (child->pressed) {
        any_line_inputs_pressed = true;
      }
    }

    if (child->type != NODE_TYPE::BUTTON) {
      continue;
    }

    if (child->get_text() != "Pick^") {
      continue;
    }

    if (child->pressed) {
      childrens[2]->set_text(std::to_string(mouse_position.x));
      childrens[4]->set_text(std::to_string(mouse_position.y));
      rect_dirty = true;
    }
  }

  // BUG: Typing on a LineInput Node should trigger recalculation of rect.
  // Workaround: We always _recalculate rect like below
  // Also, if LineInput Node isn't selected then there's no change i.e it's
  // rect can change. any_line_inputs_pressed -> workaround variable.
  if (rect_dirty || any_line_inputs_pressed) {
    _recalculate_rect();
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    // Left to drag, right to undrag.
    // We may use left to undrag as well, but those clicks occur so
    // fast, mostly it causes toggle on/off/on.. conditions.
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (_any_node_already_pressed()) {
        _deselect_all_nodes();
      }

      if (_process_left_click_on_children(event)) {
        return;
      }

      // No any internal blocks were clicked.
      if (!dragging && isMouseOverSprite(block_rect)) {
        dragging = true;
      }

      /*
            // Process Node's callbacks, if any;
            bool callback_called = false;
            for (auto &child : childrens) {
              if (child.type == NODE_TYPE::BUTTON && child.text == "Pick^") {
                if (!child.pressed) {
                  //  child.pressed = true;
                  callback_called = true;
                }
                //          callback_called = true;
              }
              // TODO: Check if mouse is over the sprite.
              //  if (child.callback) {
              //   child.callback();
              //  callback_called = true;
              //}
            }

            if (callback_called) {
              return;
            }
      */
    } else if (event.mouseButton.button == sf::Mouse::Right) {
      // TODO: All clicks outside a block should invalidate pressed state of
      // any NODE.
      _deselect_all_nodes();

      if (dragging) {
        dragging = false;
      }
    }
  }
}
