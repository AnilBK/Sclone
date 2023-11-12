#include "TabBar.hpp"

void TabBar::set_pos(sf::Vector2f pos) {
  tab_bg.setPosition(pos);
  tab_bg_pos = pos;
}

void TabBar::set_size(sf::Vector2f size) {
  tab_bg.setSize(size);
  tab_bg_size = size;
}

TabBar::TabBar(sf::Vector2f position, sf::Vector2f size) {
  set_pos(position);
  set_size(size);

  tab_bg.setFillColor(sf::Color(206, 207, 131));

  tab_container.padding.x = 0;
  tab_container.setPosition(position);
}

void TabBar::add_tab(std::string tab_name) {
  auto current_btn_index = tab_bar_buttons.size();
  std::function<void()> select_it = [this, current_btn_index]() {
    select_tab(current_btn_index);
  };

  std::shared_ptr<Button> btn(
      new Button(tab_name, sf::Vector2f(), TEXT_ALIGN::EXPAND_BUTTON_TO_TEXT));
  btn->clicked_callback = select_it;
  btn->default_fill_color = btn_default_fill_color;
  btn->pressed_fill_color = btn_selected_fill_color;
  btn->set_outline_thickness(0);

  tab_bar_buttons.push_back(btn);
  tab_container.add_child(*tab_bar_buttons.back());

  // Whenever we add a new tab button, this new tab button may have a slighlty
  // larger button size & the size of the container may change, depending upon
  // the size of the tab button. So, we iterate over all the buttons & make
  // their height equal to the height of the container, so all the texts inside
  // the buttons will be aligned & all the buttons have same height.
  auto tab_container_size = tab_container.rect_size();
  for (auto &tab_btn : tab_bar_buttons) {
    tab_btn->set_button_size({tab_btn->rect_size().x, tab_container_size.y});
  }

  // Skip the titlebar for tab_bg drawing.
  // tab_bg will be drawn below the title bar containing all those tab buttons.
  tab_bg.setPosition(tab_bg_pos + sf::Vector2f(0, tab_container_size.y));
  tab_bg.setSize(tab_bg_size - sf::Vector2f(0, tab_container_size.y));

  tab_bar_scroll_value.push_back(0.0f);
}

void TabBar::select_tab(std::size_t new_tab_id) {
  // Unselect all.
  // Buttons can have individual colors.
  // But let's not give them.
  // Since they are a part of a tab bar, every buttons should have the same
  // color.

  currently_selected_tab = new_tab_id;

  std::size_t i = 0;
  for (auto &btn : tab_bar_buttons) {
    if (i == new_tab_id) {
      btn.get()->clicked = true;
      btn.get()->set_pressed(true);
    } else {
      btn.get()->clicked = false;
      btn.get()->set_pressed(false);
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
  if ((p_delta < 0 && can_scroll_up) || (p_delta > 0 && can_scroll_down)) {
    tab_bar_scroll_value.at(currently_selected_tab) += p_delta;
  }
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
