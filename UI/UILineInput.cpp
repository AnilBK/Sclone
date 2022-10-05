#include "UILineInput.hpp"

sf::Vector2f UILineInput::getPosition() { return text.getPosition(); }

void UILineInput::setPosition(sf::Vector2f pos) { text.setPosition(pos); }

sf::Vector2f UILineInput::rect_size() {
  auto size = text.rect_size();
  float x = std::max(min_size.x, size.x);
  float y = std::max(min_size.y, size.y);
  return {x, y};
}

void UILineInput::set_text(const std::string &p_input_text) {
  input_text = p_input_text;
  emit_signal("resized");
}

std::string UILineInput::get_text() { return prefix + input_text; }

void UILineInput::RenderDebug() {
  sf::RectangleShape debug_shape;
  // debug_shape.setPosition(text.getGlobalBounds().left,
  //   text.getGlobalBounds().top);
  debug_shape.setPosition(getPosition());
  debug_shape.setSize(rect_size());
  debug_shape.setFillColor(sf::Color::Green);
  debug_shape.setOutlineThickness(1.0f);
  debug_shape.setOutlineColor(sf::Color::Black);
  window.draw(debug_shape);
  //   window.draw(text);
}

void UILineInput::_draw_button(const sf::Vector2f p_position,
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

void UILineInput::Render() {
  if (line_input_active) {
    text_effect_time += clock.restart();

    if (text_effect_time >= sf::seconds(0.5f)) {
      show_cursor = !show_cursor;
      text_effect_time = sf::Time::Zero;
    }

  } else {
    show_cursor = false;
  }

  if (is_flat) {
    if (line_input_active) {
      _draw_button(getPosition(), rect_size());
    }
  } else {
    _draw_button(getPosition(), rect_size());
  }

  text.setText(get_text() + (show_cursor ? '_' : ' '));
  text.Render();
}

void UILineInput::handle_inputs(sf::Event event) {

  bool is_now_mouse_over = is_mouse_over();

  // UILineInputs don't show their input fields unless clicked.
  // So by changing the cursor look, we notify that this is the click able
  // field.

  // Mouse has just arrived over it.
  // Mouse Entered Event.
  if (!mouse_over && is_now_mouse_over) {
    if (line_input_active) {
      setCursor(sf::Cursor::Text);
    } else {
      if (is_flat) {
        setCursor(sf::Cursor::Hand);
      } else {
        setCursor(sf::Cursor::Text);
      }
    }
    // std::cout << "Mouse Hovered.\n";
  }

  // Mouse isn't hovering anymore.
  // Mouse Leave Event.
  if (mouse_over && !is_now_mouse_over) {
    setCursor(sf::Cursor::Arrow);
    // std::cout << "Mouse No Longer Hovering.\n";
  }

  mouse_over = is_now_mouse_over;

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    if (mouse_over) {
      line_input_active = true;
      setCursor(sf::Cursor::Text);
    } else {
      line_input_active = false;
    }
  }

  if (!line_input_active) {
    return;
  }

  bool resized = false;

  if (event.type == sf::Event::TextEntered) {
    if (std::isprint(event.text.unicode)) {
      input_text += event.text.unicode;
      resized = true;
    }
  } else if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::BackSpace) {
      if (!input_text.empty()) {
        input_text.pop_back();
        resized = true;
      }
    }
    if (event.key.code == sf::Keyboard::Enter) {
      // Either we can go to next line.
      // Or we perform some actions when Enter Pressed.
      if (allows_multi_line) {
        input_text += '\n';
        resized = true;
      } else {
        if (enter_pressed_callback) {
          enter_pressed_callback();
        }
      }
    }
  }

  if (resized) {
    emit_signal("resized");
  }
}
