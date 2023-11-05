#include "RectangleShapeNode.hpp"
#include "../Core/GenerateBindingsCode.hpp"
#include "../Core/RegisteredTypes.hpp"

RectangleShapeNode::RectangleShapeNode(sf::RectangleShape p_rect_shape) {
  const auto rect_pos = p_rect_shape.getPosition();

  m_shape = p_rect_shape;
  m_shape.setPosition(sf::Vector2f());
  m_shape.setOrigin(m_shape.getSize().x / 2.0f, m_shape.getSize().y / 2.0f);

  setPosition(rect_pos);
}

sf::FloatRect RectangleShapeNode::get_global_bounds() {
  return m_shape.getGlobalBounds();
}

void RectangleShapeNode::onDraw(sf::RenderTarget &target,
                                const sf::Transform &transform) const {
  const sf::Transform combined = transform;
  target.draw(m_shape, combined);
}

//////////////////////////////////////////////////////////////////////
//                      Bindings Related Code.                      //
//////////////////////////////////////////////////////////////////////
void RectangleShapeNode::bind() {
  setter_fn_type<sf::Color> set_color;
  set_color.set = [](RectangleShapeNode &circle, sf::Color new_color) {
    circle.get_shape().setFillColor(new_color);
  };

  getter_fn_type<sf::Color> get_color;
  get_color.get = [](RectangleShapeNode &circle) {
    return circle.get_shape().getFillColor();
  };

  bind_member(sf::Color, "Color", get_color, set_color);
}