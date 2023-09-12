#ifndef TRANSFORM_GIZMO_2D_HPP
#define TRANSFORM_GIZMO_2D_HPP

#include "../Globals.hpp"
#include <SFML/Graphics.hpp>

enum class GIZMO_SELECT_STATE { X, Y, CENTER, NONE };

class TransformGizmo2D {
private:
  sf::Sprite *target_sprite = nullptr;
  GIZMO_SELECT_STATE current_gizmo_state = GIZMO_SELECT_STATE::NONE;

  void _draw_gizmo_axes();
  void _draw_gizmo();

public:
  bool is_gizmo_selected();
  void _undrag_gizmos();
  void setTargetSprite(sf::Sprite *target_spr);
  void Render();
};

#endif // TRANSFORM_GIZMO_2D_HPP
