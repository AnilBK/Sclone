#include "CircleShapeNode.hpp"
#include "../Core/GenerateBindingsCode.hpp"
#include "../Core/RegisteredTypes.hpp"

CircleShapeNode::CircleShapeNode(sf::CircleShape p_circle_shape) {
  const auto circle_pos = p_circle_shape.getPosition();

  m_shape = p_circle_shape;
  m_shape.setPosition(sf::Vector2f());
  update_shape_bounds();

  setPosition(circle_pos);
}

void CircleShapeNode::update_shape_bounds() {
  auto bounds = m_shape.getGlobalBounds().getSize();
  m_shape.setOrigin(bounds.x / 2.0f, bounds.y / 2.0f);
}

void CircleShapeNode::set_radius(float p_new_radius) {
  auto &circle_shape_ref = get_shape();
  circle_shape_ref.setRadius(p_new_radius);
  update_shape_bounds();
}

sf::FloatRect CircleShapeNode::get_global_bounds() {
  return m_shape.getGlobalBounds();
}

void CircleShapeNode::onDraw(sf::RenderTarget &target,
                             const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}

//////////////////////////////////////////////////////////////////////
//                      Bindings Related Code.                      //
//////////////////////////////////////////////////////////////////////
void CircleShapeNode::bind() {
  setter_fn_type<float> set_radius;
  set_radius.set = [](CircleShapeNode &circle, float p_radius) {
    circle.set_radius(p_radius);
  };

  getter_fn_type<float> get_radius;
  get_radius.get = [](CircleShapeNode &circle) {
    return circle.get_shape().getRadius();
  };

  setter_fn_type<sf::Color> set_color;
  set_color.set = [](CircleShapeNode &circle, sf::Color new_color) {
    circle.get_shape().setFillColor(new_color);
  };

  getter_fn_type<sf::Color> get_color;
  get_color.get = [](CircleShapeNode &circle) {
    return circle.get_shape().getFillColor();
  };

  bind_member(sf::Color, "Color", get_color, set_color);
  bind_member(float, "Radius", get_radius, set_radius);
}