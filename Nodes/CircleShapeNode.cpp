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

//////////////////////////////////////////////////////////////////////
//                      Bindings Related Code.                      //
//////////////////////////////////////////////////////////////////////
template <typename T> constexpr const char *get_params() { return ""; }
template <typename T> constexpr std::size_t get_params_count() { return 1; }

#define REGISTER_TYPE(T, params, params_count)                                 \
  template <> constexpr const char *get_params<T>() { return params; }         \
  template <> constexpr std::size_t get_params_count<T>() {                    \
    return params_count;                                                       \
  }

REGISTER_TYPE(sf::Color, "r,g,b,a", 4)
REGISTER_TYPE(float, "", 1)

#undef REGISTER_TYPE

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

#define bind(T, NAME, GET, SET)                                                \
  {                                                                            \
    Property<T> property;                                                      \
    property.name = NAME;                                                      \
    property.params = get_params<T>();                                         \
    property.params_count = get_params_count<T>();                             \
    property.getter_fn = GET;                                                  \
    property.setter_fn = SET;                                                  \
    bounded_properties->push_back(property);                                   \
  }

  bind(sf::Color, "Color", get_color, set_color);
  bind(float, "Radius", get_radius, set_radius);
}