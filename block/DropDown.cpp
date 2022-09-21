#include "DropDown.hpp"

bool DropDown::is_mouse_over() {
  sf::Vector2f size = rect_size();
  return (mouse_position.x >= position.x &&
          mouse_position.x <= position.x + size.x) &&
         (mouse_position.y >= position.y &&
          mouse_position.y <= position.y + size.y);
}

void DropDown::handle_inputs(sf::Event event) {
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

  auto is_mouse_over_rect = [](sf::Vector2f pos, sf::Vector2f size) {
    return (mouse_position.x >= pos.x && mouse_position.x <= pos.x + size.x) &&
           (mouse_position.y >= pos.y && mouse_position.y <= pos.y + size.y);
  };

  int number_of_item_to_draw = dropdown_clicked ? items.size() : 1;
  sf::Vector2f draw_position = position;

  for (int i = 0; i < number_of_item_to_draw; i++) {
    auto content_size = rect_size();
    if (is_mouse_over_rect(draw_position, content_size)) {
      mouse_over_index = i;
      break;
    }
    draw_position.y += content_size.y;
  }
}

void DropDown::Render() {
  sf::Vector2f draw_position = position;

  if (!dropdown_clicked) {
    auto text = get_text();
    auto content_size = rect_size();

    sf::RectangleShape bg_rect;
    bg_rect.setPosition(draw_position);
    bg_rect.setSize(content_size);
    bg_rect.setFillColor(sf::Color::Green);

    bg_rect.setOutlineThickness(2.5);
    bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));

    window.draw(bg_rect);
    draw_text(text, draw_position);

    draw_position.y += content_size.y;

    return;
  }

  int number_of_item_to_draw = dropdown_clicked ? items.size() : 1;

  for (int i = 0; i < number_of_item_to_draw; i++) {
    auto text = get_text(i);
    auto content_size = rect_size();

    sf::RectangleShape bg_rect;
    bg_rect.setPosition(draw_position);
    bg_rect.setSize(content_size);
    bg_rect.setFillColor(sf::Color::Green);

    if (i == mouse_over_index) {
      bg_rect.setOutlineThickness(2.5);
      bg_rect.setOutlineColor(sf::Color(31, 142, 255, 255));
    } else {
      bg_rect.setOutlineThickness(0.0);
    }

    window.draw(bg_rect);
    draw_text(text, draw_position);

    draw_position.y += content_size.y;
  }
}