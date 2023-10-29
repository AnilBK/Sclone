#ifndef BLOCK_VISUALIZER_HPP
#define BLOCK_VISUALIZER_HPP

#include "../Globals.hpp"
#include "../block/Block.hpp"
#include <SFML/Graphics.hpp>

class Editor;
class EditorSprite;

/// @brief Class to visualize what a block does, when a mouse is hovered above a
/// block.
class BlockVisualizer {
private:
  Editor &editor_ref;

  sf::View *world_2d_view;
  EditorSprite *target_sprite_ptr;

  sf::Vector2f _get_sprite_position();

  void _draw_arrow_triangle(sf::Vector2f from, sf::Vector2f to,
                            sf::Color p_color = sf::Color(0, 200, 0));
  void _draw_arrow(sf::Vector2f from, sf::Vector2f to);

  void draw_debug_block_go_to_xy(const Block &block, float delta);
  void draw_debug_change_y_by(const Block &block, float delta);
  void draw_debug_change_y_by_in(const Block &block, float delta);
  void draw_debug_block_glide_to_xy(const Block &block, float delta);
  void draw_debug_glide_point_to_point(const Block &block, float delta);

  void DebugBlock(const Block &block, float delta);

public:
  BlockVisualizer(Editor &p_editor)
      : editor_ref(p_editor), world_2d_view(nullptr),
        target_sprite_ptr(nullptr) {}

  void init();

  void Update(float delta);
};

#endif
