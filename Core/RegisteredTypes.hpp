#ifndef REGISTERED_TYPES_HPP
#define REGISTERED_TYPES_HPP

#include <SFML/Graphics/Color.hpp>
#include <cstddef>

namespace RegisteredTypes {

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
} // namespace RegisteredTypes

#endif // REGISTERED_TYPES_HPP
