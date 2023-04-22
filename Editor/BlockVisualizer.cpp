#include "BlockVisualizer.hpp"
#include "../Utils.hpp"
#include "Editor.hpp"
#include <array>
#include <cmath>
#include <string>

void BlockVisualizer::init() {
  world_2d_view = editor_ref.get_world_2d_view_ptr();
}

void BlockVisualizer::_draw_arrow_triangle(sf::Vector2f from, sf::Vector2f to,
                                           sf::Color p_color) {
  float angle = MATH_UTILITIES::angle_between_points(from, to);

  sf::ConvexShape triangle;
  triangle.setPointCount(3);
  triangle.setPoint(0, {20, 0});
  triangle.setPoint(1, {0, -10});
  triangle.setPoint(2, {0, 10});
  triangle.setFillColor(p_color);
  triangle.setPosition(to);
  triangle.setRotation(angle);
  window.draw(triangle);
}

void BlockVisualizer::_draw_arrow(sf::Vector2f from, sf::Vector2f to) {
  float angle = MATH_UTILITIES::angle_between_points(from, to);

  draw_line(from, to, sf::Color::Red);

  sf::ConvexShape triangle;
  triangle.setPointCount(3);
  triangle.setPoint(0, {20, 0});
  triangle.setPoint(1, {0, -10});
  triangle.setPoint(2, {0, 10});
  triangle.setFillColor(sf::Color(0, 200, 0)); // Draw triangle at Start.
  triangle.setPosition(from);
  triangle.setRotation(angle);
  window.draw(triangle);

  triangle.setFillColor(sf::Color::Red); // Draw triangle at End.
  triangle.setPosition(to);
  window.draw(triangle);
}

void BlockVisualizer::draw_debug_block_go_to_xy(const Block &block,
                                                float delta) {
  float x = block.get_bound_value_or("x", 0.0F);
  float y = block.get_bound_value_or("y", 0.0F);

  auto pos = sf::Vector2f(x, y);

  window.setView(*world_2d_view);
  // block.is_interacted() checks for mouse inputs in it's own world.
  // so we can't set this up in the update loop.
  //  Therefore, we have to set views and reset views for every debug function.

  // Draw a small 'plus' at the point.
  draw_line(sf::Vector2f(pos.x - 50.0F, pos.y),
            sf::Vector2f(pos.x + 50.0F, pos.y), sf::Color::Black);

  draw_line(sf::Vector2f(pos.x, pos.y - 50.0F),
            sf::Vector2f(pos.x, pos.y + 50.0F), sf::Color::Red);

  window.setView(window.getDefaultView());
}

void BlockVisualizer::draw_debug_change_y_by(const Block &block, float delta) {
  auto pos = target_sprite_ptr->sprite.getPosition();
  float y = block.get_bound_value_or("y_offset", 50.0F);

  window.setView(*world_2d_view);

  _draw_arrow(pos, pos + sf::Vector2f(0, y));

  window.setView(window.getDefaultView());
}

void BlockVisualizer::draw_debug_change_y_by_in(const Block &block,
                                                float delta) {
  auto pos = target_sprite_ptr->sprite.getPosition();

  float y = block.get_bound_value_or("y_offset", 50.0F);
  float len = block.get_bound_value_or("length", 1.0F);

  window.setView(*world_2d_view);

  _draw_arrow(pos, pos + sf::Vector2f(0, y));

  static float time_elapsed = 0.0f;
  static float length = 1.0f;

  length = len;

  time_elapsed += delta;

  float weight = time_elapsed / length;

  if (time_elapsed >= length) {
    time_elapsed = 0.0f;
  }

  auto end = MATH_UTILITIES::lerp(pos, pos + sf::Vector2f(0, y), weight);
  _draw_arrow_triangle(pos, end);

  window.setView(window.getDefaultView());
}

void BlockVisualizer::draw_debug_block_glide_to_xy(const Block &block,
                                                   float delta) {
  auto pos = target_sprite_ptr->sprite.getPosition();

  const std::array<std::string, 3> queries = {"x", "y", "length"};
  const std::array<float, 3> values = {
      block.get_bound_value_or(queries[0], 0.0F),
      block.get_bound_value_or(queries[1], 0.0F),
      block.get_bound_value_or(queries[2], 1.0F)};

  float x = values[0];
  float y = values[1];
  float len = values[2];

  sf::Vector2f final_pos{x, y};

  window.setView(*world_2d_view);

  _draw_arrow(pos, final_pos);

  static float time_elapsed = 0.0f;
  static float length = 1.0f;

  length = len;

  time_elapsed += delta;

  float weight = time_elapsed / length;

  if (time_elapsed >= length) {
    time_elapsed = 0.0f;
  }

  auto end = MATH_UTILITIES::lerp(pos, final_pos, weight);
  _draw_arrow_triangle(pos, end, sf::Color(0, 200, 0));

  window.setView(window.getDefaultView());
}

void BlockVisualizer::draw_debug_glide_point_to_point(const Block &block,
                                                      float delta) {
  const std::array<std::string, 5> queries = {"x", "y", "x1", "y1", "length"};
  const std::array<float, 5> values = {
      block.get_bound_value_or(queries[0], 0.0F),
      block.get_bound_value_or(queries[1], 0.0F),
      block.get_bound_value_or(queries[2], 100.0F),
      block.get_bound_value_or(queries[3], 100.0F),
      block.get_bound_value_or(queries[4], 1.0F)};

  float x = values[0];
  float y = values[1];
  float x1 = values[2];
  float y1 = values[3];
  float len = values[4];

  window.setView(*world_2d_view);

  sf::Vector2f pos{x, y};
  sf::Vector2f final_pos{x1, y1};

  _draw_arrow(pos, final_pos);

  static float time_elapsed = 0.0f;
  static float length = 1.0f;

  length = len;

  time_elapsed += delta;

  float weight = time_elapsed / length;

  if (time_elapsed >= length) {
    time_elapsed = 0.0f;
  }

  auto end = MATH_UTILITIES::lerp(pos, final_pos, weight);
  _draw_arrow_triangle(pos, end, sf::Color(0, 200, 0));

  window.setView(window.getDefaultView());
}

void BlockVisualizer::DebugBlock(const Block &block, float delta) {
  auto function_identifier = block.function_identifier;
  if (function_identifier == "block_change_y_by") {
    draw_debug_change_y_by(block, delta);
  } else if (function_identifier == "block_change_y_by_in") {
    draw_debug_change_y_by_in(block, delta);
  } else if (function_identifier == "block_glide_to_xy") {
    draw_debug_block_glide_to_xy(block, delta);
  } else if (function_identifier == "block_glide_point_to_point") {
    draw_debug_glide_point_to_point(block, delta);
  } else if (function_identifier == "block_go_to_xy") {
    draw_debug_block_go_to_xy(block, delta);
  }
}

void BlockVisualizer::Update(float delta) {
  auto &script_editor_ref = editor_ref.script_editor;

  auto script = script_editor_ref.script;
  if (!script) {
    return;
  }

  target_sprite_ptr = editor_ref.selected_sprite_ptr();
  if (!target_sprite_ptr) {
    return;
  }

  if (world_2d_view == nullptr) {
    return;
  }

  auto script_editor_world = script_editor_ref.get_world();
  auto script_editor_view = script_editor_ref.get_view();

  bool isMouseOverScriptEditorView = isMouseOverRect(script_editor_world);

  window.setView(script_editor_view);

  for (auto &block : script->blocks) {
    bool is_mouse_over = isMouseOverScriptEditorView && block.is_mouse_over();

    if (is_mouse_over || block.any_node_already_pressed()) {
      DebugBlock(block, delta);
      window.setView(script_editor_view);
    }
  }

  window.setView(window.getDefaultView());
}
