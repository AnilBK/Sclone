#ifndef TAB_BAR_HPP
#define TAB_BAR_HPP

#include "../Globals.hpp"
#include "Button.hpp"
#include "Container.hpp"
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
  /// @brief The actual pos & size of the entire tabbar container.
  sf::Vector2f tab_bg_pos, tab_bg_size;
  sf::RectangleShape tab_bg;

  std::vector<std::shared_ptr<Button>> tab_bar_buttons;
  HBoxContainer tab_container;

  std::size_t currently_selected_tab = 0;
  std::vector<int> tab_bar_scroll_value;

  const sf::Color btn_selected_fill_color = sf::Color(206, 207, 131);
  const sf::Color btn_default_fill_color = sf::Color(144, 127, 66);

  void _add_scroll_value_to_current_tab(int p_delta);
  void _render_title();
  void _render_body();

public:
  TabBarStatus render_status = TabBarStatus::SHOW_ALL;

  bool can_scroll_up = true, can_scroll_down = true;

  TabBar(sf::Vector2f position, sf::Vector2f size);

  void set_pos(sf::Vector2f pos);

  void set_size(sf::Vector2f size);

  void add_tab(const std::string &tab_name);

  void select_tab(std::size_t new_tab_id);

  std::size_t get_currently_selected_tab() { return currently_selected_tab; }

  int get_current_tab_body_scroll();

  /// @brief Get the actual dimensions of the tab without the title buttons.
  /// @return Rect enclosing actual body.
  sf::FloatRect get_tab_body_size() {
    auto body_position = tab_bg.getPosition();
    auto body_size = tab_bg.getSize();

    return {body_position, body_size};
  }

  void handle_inputs(sf::Event event);

  void Render();
};

#endif
