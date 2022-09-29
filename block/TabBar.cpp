#include "TabBar.hpp"

TabBar::TabBar(sf::Vector2f position, sf::Vector2f size) {
  _initial_pos = position;
  _initial_size = size;
  tab_bg.setPosition(position);
  tab_bg.setSize(size);
  tab_bg.setFillColor(sf::Color(204, 204, 204));
}

void TabBar::add_tab(std::string tab_name) {
  const float btn_spacing = 30.0f;

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

void TabBar::_select_tab(int idx) {
  ERR_FAIL_COND_CRASH(idx < 0 || idx > tab_bar_buttons.size(),
                      "Invalid tab selected. Provided index: " +
                          std::to_string(idx));
  // Unselect all.
  // Buttons can have individual colors.
  // But let's not give them.
  // Since they are a part of a tab bar, every buttons should have the same
  // color.

  int i = 0;
  Button *currently_selected_tab_ptr = nullptr;
  for (auto &_tab_bar_btn : tab_bar_buttons) {
    _tab_bar_btn.clicked = false;
    _tab_bar_btn.button_fill_color = btn_default_fill_color;
    if (i == idx) {
      currently_selected_tab_ptr = &_tab_bar_btn;
    }
    i++;
  }

  // Select only that tab.
  currently_selected_tab_ptr->clicked = true;
  currently_selected_tab_ptr->button_fill_color = btn_selected_fill_color;
  currently_selected_tab = idx;
}

void TabBar::handle_inputs(sf::Event event) {
  if (event.type == sf::Event::MouseWheelMoved) {
    add_scroll_value_to_current_tab(event.mouseWheel.delta);
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

  tab_bg.setPosition(get_initial_position() + btn_size_delta);
  tab_bg.setSize(_initial_size - btn_size_delta);
}

int TabBar::get_scroll_value() {
  if (currently_selected_tab == -1) {
    return 0;
  }

  if (currently_selected_tab > tab_bar_scroll_value.size()) {
    ERR_FAIL_COND_CRASH(true, "Invalid Selected tab index.");
  }

  return tab_bar_scroll_value.at(currently_selected_tab);
}

void TabBar::add_scroll_value_to_current_tab(int p_delta) {
  if (currently_selected_tab == -1) {
    return;
  }

  tab_bar_scroll_value.at(currently_selected_tab) += p_delta;
}

void TabBar::Render() {
  window.draw(tab_bg);

  for (auto &tab_bar_btn : tab_bar_buttons) {
    tab_bar_btn.Render();
  }
}
