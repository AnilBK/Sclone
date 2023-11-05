#ifndef CIRCLE_SHAPE_NODE_HPP
#define CIRCLE_SHAPE_NODE_HPP

#include "Node.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <variant>

class CircleShapeNode : public Node {
public:
  CircleShapeNode(sf::CircleShape p_circle_shape);

  void onDraw(sf::RenderTarget &target,
              const sf::Transform &transform) const override;

  sf::FloatRect get_global_bounds() override;

  sf::CircleShape &get_shape() { return m_shape; }

  void update_shape_bounds();

  void set_radius(float p_new_radius);

  //////////////////////////////////////////////////////////////////////
  //                      Bindings Related Code.                      //
  //////////////////////////////////////////////////////////////////////
  template <class return_type> struct getter_fn_type {
    std::function<return_type(CircleShapeNode &)> get;
  };

  template <class set_type> struct setter_fn_type {
    std::function<void(CircleShapeNode &, set_type)> set;
  };

  template <class T> struct Property {
    std::string name;
    std::string params;
    std::size_t params_count;
    getter_fn_type<T> getter_fn;
    setter_fn_type<T> setter_fn;
  };

  using ColorType = Property<sf::Color>;
  using FloatType = Property<float>;

  using PropertyVariant = std::variant<ColorType, FloatType>;

  // Important : Always delete them.
  // Deletion is Handled by '_global_binder'.
  inline static std::vector<PropertyVariant> *bounded_properties =
      new std::vector<PropertyVariant>();

  static void bind();

private:
  sf::CircleShape m_shape;
};

#endif // CIRCLE_SHAPE_NODE_HPP
