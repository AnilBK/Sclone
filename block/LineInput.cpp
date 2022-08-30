#include "LineInput.hpp"

bool LineInput::is_mouse_over() {
  sf::Vector2f size = rect_size();
  return (mouse_position.x >= position.x &&
          mouse_position.x <= position.x + size.x) &&
         (mouse_position.y >= position.y &&
          mouse_position.y <= position.y + size.y);
}

void LineInput::handle_inputs(sf::Event event) {

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    if (is_mouse_over()) {
      line_input_active = true;
    } else {
      line_input_active = false;
    }
  }

  if (!line_input_active) {
    return;
  }

  if (event.type == sf::Event::TextEntered) {
    if (std::isprint(event.text.unicode))
      input_text += event.text.unicode;
  } else if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::BackSpace) {
      if (!input_text.empty())
        input_text.pop_back();
    }
    if (allows_multi_line) {
      if (event.key.code == sf::Keyboard::Return) {
        input_text += '\n';
      }
    }
  }
}

void LineInput::_draw_button(const sf::Vector2f p_position,
                             const sf::Vector2f p_size, sf::Color color) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(p_size);
  r.setFillColor(color);
  // A shadow when a button is highlighted.
  if (line_input_active) {
    r.setOutlineThickness(2.5);
    r.setOutlineColor(sf::Color(31, 142, 255, 255));
  } else {
    r.setOutlineThickness(2.0);
    r.setOutlineColor(sf::Color(91, 91, 91, 255));
  }
  window.draw(r);
}

void LineInput::Render() {

  if (line_input_active) {
    text_effect_time += clock.restart();

    if (text_effect_time >= sf::seconds(0.5f)) {
      show_cursor = !show_cursor;
      text_effect_time = sf::Time::Zero;
    }

  } else {
    show_cursor = false;
  }

  _draw_button(position, rect_size());
  draw_text(get_text() + (show_cursor ? '_' : ' '), position);
}

// We can't use text.getString(), as text object contains _ or _ to show the
// blinking cursor.
std::string LineInput::get_text() { return prefix + input_text; }
