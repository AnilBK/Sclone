#ifndef TRANSFORM_GIZMO_2D_HPP
#define TRANSFORM_GIZMO_2D_HPP

#include "../Globals.hpp"
#include "../Nodes/Node.hpp"
#include "EditorSprite.hpp"
#include "RadiusGizmoPlugin.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

enum class GIZMO_SELECT_STATE { X, Y, CENTER, SCALE, CUSTOM, NONE };

class TransformGizmo2D {
private:
  EditorSprite *target_editor_sprite = nullptr;

  Node *target_node = nullptr;

  void _update_gizmo();
  void _draw_gizmo();

  RadiusGizmoPlugin radius_gizmo = RadiusGizmoPlugin(*this);

public:
  /// @brief Offset of currently selected sprite & the mouse position, so that
  /// the sprite can be moved relative to mouse in the editor.
  sf::Vector2f offset = sf::Vector2f();

  GIZMO_SELECT_STATE current_gizmo_state = GIZMO_SELECT_STATE::NONE;

  Node *get_target_node() { return target_node; }

  /// @brief When any translation is produced by the gizmo, the editor may need
  /// to update it's labels and stuffs. Those functions are stored in this
  /// callback.
  std::function<void(bool, bool)> translation_updated_callbacks;

  bool is_gizmo_selected();
  void _undrag_gizmos();

  void setTargetEditorSprite(EditorSprite *p_target_editor_sprite);

  void Render();
};

#endif // TRANSFORM_GIZMO_2D_HPP
