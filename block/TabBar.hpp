#ifndef TAB_BAR_HPP
#define TAB_BAR_HPP

#include "../Globals.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>

enum class TabBarStatus { SHOW_ALL, SHOW_ONLY_TITLE, SHOW_ONLY_BODY };

class TabBar {

private:
  sf::RectangleShape tab_bg;
  std::vector<Button> tab_bar_buttons;
  std::vector<int> tab_bar_scroll_value;

  sf::Font _font;
  sf::Vector2f _initial_pos, _initial_size;

  const sf::Color btn_selected_fill_color = sf::Color(200, 200, 200);
  const sf::Color btn_default_fill_color = sf::Color::Green;

  void add_scroll_value_to_current_tab(int p_delta);
  void _render_title();
  void _render_body();

public:
  TabBarStatus render_status = TabBarStatus::SHOW_ALL;
  int currently_selected_tab = -1;
  TabBar(sf::Vector2f position, sf::Vector2f size);

  void set_font(const sf::Font &font) { _font = font; }
  sf::Vector2f get_initial_position() { return _initial_pos; }
  sf::Vector2f get_visible_tab_position() { return tab_bg.getPosition(); }

  void add_tab(std::string tab_name);
  void handle_inputs(sf::Event event);
  void _select_tab(int idx);
  void recalculate_post_add_tabs();
  int get_scroll_value();
  void Render();
};

#endif
