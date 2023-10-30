#ifndef RADIUS_GIZMO_PLUGIN_HPP
#define RADIUS_GIZMO_PLUGIN_HPP

#include "../Globals.hpp"
#include "../Nodes/Node.hpp"
#include <SFML/Graphics.hpp>

class TransformGizmo2D;

class RadiusGizmoPlugin {
private:
  sf::CircleShape c;

  TransformGizmo2D &parent_gizmo;
  Node *get_target_node();

  bool is_gizmo_selected = false;

  /// @brief Check if the currently selected node can use this gizmo.
  /// @return True if currently selected node is a CircleShapeNode.
  bool is_target_node_valid();

public:
  bool can_be_selected();

  bool is_selected();

  /// @brief Just draw the gizmo components.
  void Render();

  /// @brief Perform various transformations on the node, for which the gizmo
  /// was created.
  void Update();

  void SelectGizmo();

  void DeselectGizmo();

  RadiusGizmoPlugin(TransformGizmo2D &p_gizmo) : parent_gizmo(p_gizmo) {
    c.setRadius(10);
    c.setFillColor(sf::Color::Magenta);
    c.setOrigin(c.getGlobalBounds().getSize().x / 2.0f,
                c.getGlobalBounds().getSize().y / 2.0f);
  }
};

#endif // RADIUS_GIZMO_PLUGIN_HPP
