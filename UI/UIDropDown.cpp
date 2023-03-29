#include "UIDropDown.hpp"

void UIDropDown::add_item(const std::string &str) {
  std::shared_ptr<UILabel> btn(new UILabel(str));
  // Cache the size of the largest label.
  auto text_size = btn->rect_size();
  largest_item_rect_size.x = std::max(largest_item_rect_size.x, text_size.x);
  largest_item_rect_size.y = std::max(largest_item_rect_size.y, text_size.y);

  items.push_back(btn);
}

void UIDropDown::add_items(std::initializer_list<std::string> options) {
  for (const auto &option : options) {
    add_item(option);
  }
}

void UIDropDown::_compute_maximum_size_for_items() {
  // Use a temporary label to get the max size of the characters.
  UILabel l("ABCDE");
  auto max_char_height = UILabel::max_character_size(l.get_label());
  largest_item_rect_size.y = max_char_height;
  largest_item_rect_size.y += 3; // Padding.
}

sf::Vector2f UIDropDown::getPosition() { return position; }

void UIDropDown::setPosition(sf::Vector2f pos) {
  position = pos;
  base_label.setPosition(position);

  sf::Vector2f draw_position = position;
  const auto content_size = rect_size();

  for (auto &item : items) {
    draw_position.y += content_size.y;
    item->setPosition(draw_position);
  }
}

sf::Vector2f UIDropDown::rect_size() { return largest_item_rect_size; }

void UIDropDown::handle_inputs(sf::Event event) {
  // Only the top rect.
  mouse_over = is_mouse_over();

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    if (mouse_over) {
      dropdown_clicked = !dropdown_clicked;
    } else {
      // Clicked anywhere.
      // So just select that item.
      if (mouse_over_index != -1) {
        select_item(mouse_over_index);
        mouse_over_index = -1;
        dropdown_clicked = false;
      }
    }
  }

  if (dropdown_clicked) {
    // Find the item over which the mouse is being hovered.
    const auto content_size = rect_size();
    for (std::size_t i = 0; i < items.size(); i++) {
      sf::FloatRect bounds{items.at(i)->getPosition(), content_size};
      if (isCachedMousePosOverRect(bounds)) {
        mouse_over_index = i;
        break;
      }
    }
  }
}

void UIDropDown::draw_item(std::size_t idx) {
  auto &label_ref = items.at(idx);

  sf::RectangleShape bg_rect;
  bg_rect.setPosition(label_ref->getPosition());
  bg_rect.setSize(rect_size());
  bg_rect.setFillColor(sf::Color::Green);
  bg_rect.setOutlineThickness(1.0);
  bg_rect.setOutlineColor(sf::Color::Black);

  if (idx == mouse_over_index) {
    bg_rect.setOutlineThickness(2.5);
    bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));
  }

  window.draw(bg_rect);
  label_ref->Render();
}

void UIDropDown::_draw_base_button() {
  sf::RectangleShape bg_rect;
  bg_rect.setPosition(position);
  bg_rect.setSize(rect_size());
  bg_rect.setFillColor(sf::Color::Green);
  bg_rect.setOutlineThickness(2.5);
  bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));
  window.draw(bg_rect);

  base_label.Render();
}

void UIDropDown::_draw_whole_list() {
  for (std::size_t i = 0; i < items.size(); i++) {
    draw_item(i);
  }
}

void UIDropDown::Render() {
  _draw_base_button();
  if (dropdown_clicked) {
    _draw_whole_list();
  }
}
