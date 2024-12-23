#include "Block/Block.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>

namespace STRING_UTILITIES {
void replaceAll(std::string &s, const std::string &search,
                const std::string &replace);

void remove_first_occurence(std::string &s, char ch);

void remove_last_occurence(std::string &s, char ch);

std::vector<std::string> SplitString(std::string &input, char split_character);

template <class T>
std::string format_string_with_precision(T value, int precision);
} // namespace STRING_UTILITIES

namespace MATH_UTILITIES {

[[nodiscard]] bool is_approx_zero(sf::Vector2f p_vec);

[[nodiscard]] sf::Vector2f round(const sf::Vector2f p_vec);

[[nodiscard]] sf::FloatRect merge_rects(sf::FloatRect rect,
                                        sf::FloatRect p_rect);

[[nodiscard]] float distance_between(sf::Vector2f p1, sf::Vector2f p2);

[[nodiscard]] sf::Vector2f lerp(const sf::Vector2f &start,
                                const sf::Vector2f &end, float lerpValue);

[[nodiscard]] float angle_between_points(sf::Vector2f p_from,
                                         sf::Vector2f p_to);

[[nodiscard]] float str_to_float(const std::string &str,
                                 float default_value = 1.0f);

[[nodiscard]] float range_to_percent(float p_min, float p_max, float p_value);
} // namespace MATH_UTILITIES

namespace BLOCK_UTILITIES {
bool is_forever_block(const Block &block);

bool is_input_block(const Block &block);

bool is_program_starts_block(const Block &block);
} // namespace BLOCK_UTILITIES
