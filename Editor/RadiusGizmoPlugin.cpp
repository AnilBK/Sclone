#include "RadiusGizmoPlugin.hpp"
#include "../Nodes/CircleShapeNode.hpp"
#include "../Utils.hpp"
#include "TransformGizmo2D.hpp"

Node *RadiusGizmoPlugin::get_target_node() {
  return parent_gizmo.get_target_node();
}
bool RadiusGizmoPlugin::is_target_node_valid() {
  auto circle = dynamic_cast<CircleShapeNode *>(get_target_node());
  return circle ? true : false;
}

bool RadiusGizmoPlugin::can_be_selected() {
  return is_target_node_valid() && isMouseOverSprite(c);
}

bool RadiusGizmoPlugin::is_selected() { return is_gizmo_selected; }

void RadiusGizmoPlugin::SelectGizmo() { is_gizmo_selected = true; }

void RadiusGizmoPlugin::DeselectGizmo() { is_gizmo_selected = false; }

void RadiusGizmoPlugin::Render() {
  auto circle = dynamic_cast<CircleShapeNode *>(get_target_node());
  if (!circle) {
    return;
  }

  auto line_start = circle->getPosition();

  c.setPosition(line_start + sf::Vector2f(0, circle->get_shape().getRadius()));
  if (isMouseOverSprite(c)) {
    c.setOutlineThickness(2.0f);
    c.setOutlineColor(sf::Color::Black);
  } else {
    c.setOutlineThickness(0);
  }

  window.draw(c);
}

void RadiusGizmoPlugin::Update() {
  if (!is_selected()) {
    return;
  }

  auto circle = dynamic_cast<CircleShapeNode *>(get_target_node());
  if (!circle) {
    return;
  }

  auto rad = MATH_UTILITIES::distance_between(get_mouse_position(),
                                              circle->getPosition());

  auto &circle_shape_ref = circle->get_shape();
  auto circle_bounds = circle_shape_ref.getGlobalBounds().getSize();

  circle_shape_ref.setRadius(rad);
  circle_shape_ref.setOrigin(circle_bounds.x / 2.0f, circle_bounds.y / 2.0f);
}