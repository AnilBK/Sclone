#include "CircleShapeNode.hpp"

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

void CircleShapeNode::bind() {
  auto bind_property = [](const std::string &property_name, fn_type getter_fn) {
    CircleShapeNode::bounded_properties->push_back({property_name, getter_fn});
  };

  fn_type get_radius_fn;
  get_radius_fn = [](CircleShapeNode &circle) {
    return circle.get_shape().getRadius();
  };

  fn_type get_scale_x;
  get_scale_x = [](CircleShapeNode &circle) {
    return circle.get_shape().getScale().x;
  };

  bind_property("Radius", get_radius_fn);
  bind_property("Scale(x)", get_scale_x);
}