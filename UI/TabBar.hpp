#ifndef TAB_BAR_HPP
#define TAB_BAR_HPP

#include "../Globals.hpp"
#include "Container.hpp"
#include "UIButton.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

enum class TabBarStatus { SHOW_ALL, SHOW_ONLY_TITLE, SHOW_ONLY_BODY };

class TabBar {
  /*
   vvv   tab_bg       vvv
  .-----------------------.
  . Control, Motion,....  .
  .-----------------------.
  .                       .
  .                       .
  .                       .
  .-----------------------.
   ^^^   tab_bg       ^^^
  */
private:
  sf::RectangleShape tab_bg;
  std::vector<std::shared_ptr<UIButton>> tab_bar_buttons;
  HBoxContainer tab_container;

  std::size_t currently_selected_tab = 0;
  std::vector<int> tab_bar_scroll_value;

  float btn_size_y_cached = 40.0;

  const sf::Color btn_selected_fill_color = sf::Color(206, 207, 131);
  const sf::Color btn_default_fill_color = sf::Color(144, 127, 66);

  void _add_scroll_value_to_current_tab(int p_delta);
  void _render_title();
  void _render_body();

public:
  TabBarStatus render_status = TabBarStatus::SHOW_ALL;

  TabBar(sf::Vector2f position, sf::Vector2f size);

  void set_pos(sf::Vector2f pos) { tab_bg.setPosition(pos); }

  void set_size(sf::Vector2f size) { tab_bg.setSize(size); }

  void add_tab(std::string tab_name);

  void select_tab(std::size_t new_tab_id);

  std::size_t get_currently_selected_tab() { return currently_selected_tab; }

  int get_current_tab_body_scroll();

  /// @brief Get the actual dimensions of the tab without the title buttons.
  /// @return Rect enclosing actual body.
  sf::FloatRect get_tab_body_size() {
    auto body_position = tab_bg.getPosition();
    auto body_size = tab_bg.getSize();

    // tab_bg contains title bar buttons as well(Refer to the figure above).
    // so, subtract them to get the dimensions of the body.
    body_position.y += btn_size_y_cached;
    body_size.y -= btn_size_y_cached;

    return {body_position, body_size};
  }

  sf::Vector2f get_actual_body_position() { return sf::Vector2f(); }

  sf::Vector2f get_actual_body_size() { return sf::Vector2f(); }

  void handle_inputs(sf::Event event);

  void Render();
};

#endif
