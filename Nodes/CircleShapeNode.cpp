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
  auto bind_property = [](const std::string &property_name,
                          getter_fn_type getter_fn, setter_fn_type setter_fn) {
    CircleShapeNode::bounded_properties->push_back(
        {property_name, getter_fn, setter_fn});
  };

  getter_fn_type get_radius_fn;
  get_radius_fn = [](CircleShapeNode &circle) {
    return circle.get_shape().getRadius();
  };

  setter_fn_type set_radius_fn;
  set_radius_fn = [](CircleShapeNode &circle, float new_value) {
    circle.set_radius(new_value);
  };

  getter_fn_type get_scale_x_fn;
  get_scale_x_fn = [](CircleShapeNode &circle) {
    return circle.get_shape().getScale().x;
  };

  setter_fn_type set_scale_x_fn;
  set_scale_x_fn = [](CircleShapeNode &circle, float new_value) {
    auto &shape_ref = circle.get_shape();
    auto current_scale = shape_ref.getScale();
    shape_ref.setScale(sf::Vector2f(new_value, current_scale.y));
  };

  bind_property("Radius", get_radius_fn, set_radius_fn);
  bind_property("Scale(x)", get_scale_x_fn, set_scale_x_fn);
}