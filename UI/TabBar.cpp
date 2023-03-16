#include "TabBar.hpp"

TabBar::TabBar(sf::Vector2f position, sf::Vector2f size)
    : _initial_pos(position), _initial_size(size) {
  tab_bg.setPosition(position);
  tab_bg.setSize(size);
  tab_bg.setFillColor(sf::Color(206, 207, 131));
}

void TabBar::add_tab(std::string tab_name) {
  // To Draw a block next to previous blocks.
  sf::Vector2f offset{0.0f, 0.0f};
  for (auto &tab_bar_btn : tab_bar_buttons) {
    offset.x += tab_bar_btn.rect_size().x;
    offset.x += btn_spacing;
  }

  Button BTN_tab_button(tab_name, font);
  BTN_tab_button.setPosition(get_initial_position() + offset);
  BTN_tab_button.button_fill_color = btn_default_fill_color;

  int current_btn_index = tab_bar_buttons.size();
  std::function<void()> select_it = [this, current_btn_index]() {
    _select_tab(current_btn_index);
  };
  BTN_tab_button.clicked_callback = select_it;

  tab_bar_buttons.push_back(BTN_tab_button);
  tab_bar_scroll_value.push_back(0.0f);
}

void TabBar::_select_tab(int new_tab_id) {
  ERR_CRASH_IF(new_tab_id < 0 || new_tab_id > tab_bar_buttons.size(),
               "Invalid tab selected. Provided index: " +
                   std::to_string(new_tab_id));
  // Unselect all.
  // Buttons can have individual colors.
  // But let's not give them.
  // Since they are a part of a tab bar, every buttons should have the same
  // color.

  currently_selected_tab = new_tab_id;

  int i = 0;
  for (auto &_tab_bar_btn : tab_bar_buttons) {
    if (i == new_tab_id) {
      _tab_bar_btn.clicked = true;
      _tab_bar_btn.button_fill_color = btn_selected_fill_color;
    } else {
      _tab_bar_btn.clicked = false;
      _tab_bar_btn.button_fill_color = btn_default_fill_color;
    }
    i++;
  }
}

void TabBar::handle_inputs(sf::Event event) {
  if (render_status != TabBarStatus::SHOW_ONLY_TITLE &&
      isMouseOverSprite(tab_bg)) {
    if (event.type == sf::Event::MouseWheelMoved) {
      add_scroll_value_to_current_tab(event.mouseWheel.delta);
    }
  }

  for (auto &tab_bar_btn : tab_bar_buttons) {
    tab_bar_btn.handle_inputs(event);
    // Selecting a tab is handled by the button callback, which is assigned at
    // the top.
  }
}

void TabBar::recalculate_post_add_tabs() {
  if (tab_bar_buttons.empty()) {
    return;
  }

  // Resize the tab bar, so buttons are at the top.
  // As soon as the button ends, the tab bg is drawn.
  auto btn_size_delta = sf::Vector2f(0, 0);

  for (auto &tab_bar_btn : tab_bar_buttons) {
    btn_size_delta.y = std::max(btn_size_delta.y, tab_bar_btn.rect_size().y);
  }

  // Fit all the buttons to same height.
  for (auto &tab_bar_btn : tab_bar_buttons) {
    auto tab_bar_size = tab_bar_btn.rect_size();
    tab_bar_btn.fit_to_size({tab_bar_size.x, btn_size_delta.y});
  }

  tab_bg.setPosition(get_initial_position() + btn_size_delta);
  tab_bg.setSize(_initial_size - btn_size_delta);
}

int TabBar::get_scroll_value() {
  if (currently_selected_tab == -1) {
    return 0;
  }

  if (currently_selected_tab > tab_bar_scroll_value.size()) {
    ERR_CRASH_IF(true, "Invalid Selected tab index.");
  }

  return tab_bar_scroll_value.at(currently_selected_tab);
}

void TabBar::add_scroll_value_to_current_tab(int p_delta) {
  if (currently_selected_tab == -1) {
    return;
  }

  tab_bar_scroll_value.at(currently_selected_tab) += p_delta;
}

void TabBar::set_spacing_between_tab_btns(float spacing) {
  btn_spacing = spacing;

  if (tab_bar_buttons.empty()) {
    return;
  }

  // Just-ReOrient the Added buttons.
  sf::Vector2f offset{0.0f, 0.0f};
  for (auto &tab_bar_btn : tab_bar_buttons) {
    tab_bar_btn.setPosition({offset.x, tab_bar_btn.getPosition().y});
    offset.x += tab_bar_btn.rect_size().x;
    offset.x += btn_spacing;
  }
}

void TabBar::_render_title() {
  for (auto &tab_bar_btn : tab_bar_buttons) {
    tab_bar_btn.Render();
  }
}

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
