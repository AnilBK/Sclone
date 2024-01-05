#include "UIDropDown.hpp"

void UIDropDown::_add_item(const std::string &str) {
  auto ui_btn = std::make_shared<UIButton>(str);
  ui_btn.get()->left_padding = 0.0F;
  ui_btn.get()->set_text_align(TEXT_ALIGN::LEFT);

  list_item_buttons.push_back(ui_btn);
  drop_down_vbox.add_child(*ui_btn.get());
}

void UIDropDown::_add_items(std::initializer_list<std::string> options) {
  for (const auto &option : options) {
    _add_item(option);
  }

  for (const auto &list_item_btn : list_item_buttons) {
    auto btn_size = list_item_btn.get()->rect_size();
    largest_item_rect_size.x = std::max(largest_item_rect_size.x, btn_size.x);
    largest_item_rect_size.y = std::max(largest_item_rect_size.y, btn_size.y);
  }

  // Set the size of all the buttons in the DropDown to the largest button width
  // & height.
  for (auto &list_item_btn : list_item_buttons) {
    list_item_btn.get()->set_button_size(largest_item_rect_size);
  }

  base_button.set_button_size(largest_item_rect_size);
}

sf::Vector2f UIDropDown::getPosition() { return position; }

void UIDropDown::setPosition(sf::Vector2f pos) {
  position = pos;

  base_button.setPosition(pos);
  drop_down_vbox.setPosition(pos + sf::Vector2f(0, largest_item_rect_size.y));
}

sf::Vector2f UIDropDown::rect_size() {
  return largest_item_rect_size;
  // Same thing.
  // return base_button.rect_size();
}

void UIDropDown::handle_inputs(sf::Event event) {
  base_button.handle_inputs(event);

  if (expanded) {
    drop_down_vbox.handle_inputs(event);

    bool was_left_clicked = event.type == sf::Event::MouseButtonReleased &&
                            event.mouseButton.button == sf::Mouse::Left;

    // Don't consume if we pressed in the base_button, as, if base_button was
    // pressed, then base_button handles the toggling of this DropDown on click
    // and we shouldn't toggle again. All this workaround because we don't
    // consume inputs.

    if (was_left_clicked && expanded && !base_button.is_mouse_over()) {
      // If any dropdown button was pressed above then expanded = false;
      // Still expanded means, we pressed outside the drop_down_vbox.
      toggle_fold();
    }
  }
}

void UIDropDown::_draw_base_button() { base_button.Render(); }

void UIDropDown::_draw_whole_list() {
  if (expanded) {
    drop_down_vbox.Render();
  }
}

void UIDropDown::Render() {
  if (!callbacks_registered) {
    register_callbacks();
  }

  _draw_base_button();
  _draw_whole_list();
}

void UIDropDown::register_callbacks() {
  // Serious BUG ???
  // Setting all these callbacks in constructors work for
  // UIDropdown object, but when those objects are used as member of
  // DropDownNode of NodeBaseClass, then these std::functions act differently.
  // For e.g : when the toggle_fold() is called, it doesn't change the
  // 'expanded' value, instead the expanded value is changed to 97 or some
  // random number.

  base_button.clicked_callback = [this]() { toggle_fold(); };

  // These functions could be set up in _add_item, but because of the bug
  // explained above, we have to do it here.
  std::size_t c = 0;
  for (auto &list_item_btn : list_item_buttons) {
    auto idx = c;
    list_item_btn.get()->clicked_callback = [&, idx]() {
      select_item_and_fold(idx);
    };
    c++;
  }

  callbacks_registered = true;
}