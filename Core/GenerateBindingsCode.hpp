#ifndef GENERATE_BINDINGS_CODE_HPP
#define GENERATE_BINDINGS_CODE_HPP

#define GENERATE_BINDINGS_CODE(T)                                              \
  template <class return_type> struct getter_fn_type {                         \
    std::function<return_type(T &)> get;                                       \
  };                                                                           \
                                                                               \
  template <class set_type> struct setter_fn_type {                            \
    std::function<void(T &, set_type)> set;                                    \
  };                                                                           \
                                                                               \
  template <class U> struct Property {                                         \
    std::string name;                                                          \
    std::string params;                                                        \
    std::size_t params_count;                                                  \
    getter_fn_type<U> getter_fn;                                               \
    setter_fn_type<U> setter_fn;                                               \
  };                                                                           \
                                                                               \
  using ColorType = Property<sf::Color>;                                       \
  using FloatType = Property<float>;                                           \
                                                                               \
  using PropertyVariant = std::variant<ColorType, FloatType>;                  \
                                                                               \
  inline static std::vector<PropertyVariant> *bounded_properties =             \
      new std::vector<PropertyVariant>();                                      \
                                                                               \
  static void bind();

#define bind_member(T, NAME, GET, SET)                                         \
  {                                                                            \
    Property<T> property;                                                      \
    property.name = NAME;                                                      \
    property.params = RegisteredTypes::get_params<T>();                        \
    property.params_count = RegisteredTypes::get_params_count<T>();            \
    property.getter_fn = GET;                                                  \
    property.setter_fn = SET;                                                  \
    bounded_properties->push_back(property);                                   \
  }

#endif // GENERATE_BINDINGS_CODE_HPP
