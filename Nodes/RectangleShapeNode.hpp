#ifndef RECTANGLE_SHAPE_NODE_HPP
#define RECTANGLE_SHAPE_NODE_HPP

#include "../Core/GenerateBindingsCode.hpp"
#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <variant>
#include <vector>

class RectangleShapeNode : public Node {
public:
  RectangleShapeNode(sf::RectangleShape p_rect_shape);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::FloatRect get_global_bounds() override;

  sf::RectangleShape &get_shape() { return m_shape; }

  //////////////////////////////////////////////////////////////////////
  //                      Bindings Related Code.                      //
  //////////////////////////////////////////////////////////////////////
  GENERATE_BINDINGS_CODE(RectangleShapeNode)

private:
  sf::RectangleShape m_shape;
};

#endif // RECTANGLE_SHAPE_NODE_HPP
