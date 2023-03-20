#include "TabBar.hpp"

TabBar::TabBar(sf::Vector2f position, sf::Vector2f size) {
  tab_bg.setPosition(position);
  tab_bg.setSize(size);
  tab_bg.setFillColor(sf::Color(206, 207, 131));

  tab_container.padding.x = 3.0F;
  tab_container.setPosition(position);
}

void TabBar::add_tab(std::string tab_name) {
  auto current_btn_index = tab_bar_buttons.size();
  std::function<void()> select_it = [this, current_btn_index]() {
    select_tab(current_btn_index);
  };

  std::shared_ptr<UIButton> btn(new UIButton(tab_name));
  btn->clicked_callback = select_it;
  btn->button_fill_color = btn_default_fill_color;

  // Cache the size of buttons, so that later we can subtract it from tab_bg to
  // get the size of tab body.
  btn_size_y_cached = std::max(btn_size_y_cached, btn.get()->rect_size().y);

  tab_bar_buttons.push_back(btn);
  tab_container.add_child(*tab_bar_buttons.back());

  tab_bar_scroll_value.push_back(0.0f);
}

void TabBar::select_tab(std::size_t new_tab_id) {
  // Unselect all.
  // Buttons can have individual colors.
  // But let's not give them.
  // Since they are a part of a tab bar, every buttons should have the same
  // color.

  currently_selected_tab = new_tab_id;

  int i = 0;
  for (auto &btn : tab_bar_buttons) {
    if (i == new_tab_id) {
      btn.get()->clicked = true;
      btn.get()->button_fill_color = btn_selected_fill_color;
    } else {
      btn.get()->clicked = false;
      btn.get()->button_fill_color = btn_default_fill_color;
    }
    i++;
  }
}

void TabBar::handle_inputs(sf::Event event) {
  if (render_status != TabBarStatus::SHOW_ONLY_TITLE &&
      isMouseOverSprite(tab_bg)) {
    if (event.type == sf::Event::MouseWheelMoved) {
      _add_scroll_value_to_current_tab(event.mouseWheel.delta);
    }
  }

  tab_container.handle_inputs(event);
}

int TabBar::get_current_tab_body_scroll() {
  if (currently_selected_tab > tab_bar_scroll_value.size()) {
    ERR_CRASH_IF(true, "Invalid Selected tab index.");
  }

  return tab_bar_scroll_value.at(currently_selected_tab);
}

void TabBar::_add_scroll_value_to_current_tab(int p_delta) {
  tab_bar_scroll_value.at(currently_selected_tab) += p_delta;
}

void TabBar::_render_title() { tab_container.Render(); }

void TabBar::_render_body() { window.draw(tab_bg); }

void TabBar::Render() {
  switch (render_status) {
  case TabBarStatus::SHOW_ONLY_TITLE:
    _render_title();
    break;

  case TabBarStatus::SHOW_ONLY_BODY:
    _render_body();
    break;

  case TabBarStatus::SHOW_ALL:
    _render_body();
    _render_title();

  default:
    break;
  }
}
