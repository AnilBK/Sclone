#include "UIDropDown.hpp"

sf::Vector2f UIDropDown::getPosition() { return position; }

void UIDropDown::setPosition(sf::Vector2f pos) { position = pos; }

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
        currently_selected_index = mouse_over_index;
        mouse_over_index = -1;
        dropdown_clicked = false;
      }
    }
  }

  if (dropdown_clicked) {
    // Find the item over which the mouse is being hovered.
    sf::Vector2f draw_position = position;
    const auto content_size = rect_size();

    for (int i = 0; i < items.size(); i++) {
      draw_position.y += content_size.y;
      if (isCachedMousePosOverRect(
              sf::FloatRect(draw_position, content_size))) {
        mouse_over_index = i;
        break;
      }
    }
  }
}

void UIDropDown::_draw_base_button() {
  const auto content_size = rect_size();

  sf::RectangleShape bg_rect;
  bg_rect.setPosition(position);
  bg_rect.setSize(content_size);
  bg_rect.setFillColor(sf::Color::Green);
  bg_rect.setOutlineThickness(2.5);
  bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));

  window.draw(bg_rect);
  draw_text(get_text(), position);
}

void UIDropDown::_draw_whole_list() {
  sf::Vector2f draw_position = position;
  const auto content_size = rect_size();

  for (int i = 0; i < items.size(); i++) {
    draw_position.y += content_size.y;

    sf::RectangleShape bg_rect;
    bg_rect.setPosition(draw_position);
    bg_rect.setSize(content_size);
    bg_rect.setFillColor(sf::Color::Green);

    if (i == mouse_over_index) {
      bg_rect.setOutlineThickness(2.5);
      bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));
    }

    window.draw(bg_rect);
    draw_text(items[i], draw_position);
  }
}

void UIDropDown::Render() {
  _draw_base_button();
  if (dropdown_clicked) {
    _draw_whole_list();
  }
}

void UIDropDown::_compute_largest_item_rect_size() {
  sf::Vector2f max_text_size{0.0f, 0.0f};
  for (const auto &item : items) {
    auto text_size = text_rect_size(item);
    max_text_size.x = std::max(max_text_size.x, text_size.x);
    max_text_size.y = std::max(max_text_size.y, text_size.y);
  }
  largest_item_rect_size = max_text_size;

  // TODO?? BUG: Add these extra 10 Pixels, because we have a bug in
  // text_rect_size(), that it returns a smaller rect size.
  largest_item_rect_size += sf::Vector2f(0, 10);
}