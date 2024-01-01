#include "Utils.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace STRING_UTILITIES {
// https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
void replaceAll(std::string &s, const std::string &search,
                const std::string &replace) {
  for (size_t pos = 0;; pos += replace.length()) {
    // Locate the substring to replace
    pos = s.find(search, pos);
    if (pos == std::string::npos)
      break;
    // Replace by erasing and inserting
    s.erase(pos, search.length());
    s.insert(pos, replace);
  }
}

void remove_first_occurence(std::string &s, char ch) {
  auto pos = s.find(ch);
  if (pos != std::string::npos) {
    s.erase(pos);
  }
}

void remove_last_occurence(std::string &s, char ch) {
  auto pos = s.rfind(ch);
  if (pos != std::string::npos) {
    s.erase(pos);
  }
}

std::vector<std::string> SplitString(std::string &input, char split_character) {
  std::istringstream iss(input);

  std::string token;
  std::vector<std::string> tokens;

  while (std::getline(iss, token, split_character)) {
    tokens.push_back(token);
  }

  return tokens;
}

template <class T>
std::string format_string_with_precision(T value, int precision) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(precision) << value;
  return oss.str();
}

} // namespace STRING_UTILITIES

namespace MATH_UTILITIES {

bool is_approx_zero(sf::Vector2f p_vec) {
  constexpr float EPSILON = 0.00001f;
  return (fabs(p_vec.x) < EPSILON) && (fabs(p_vec.y) < EPSILON);
}

sf::Vector2f round(const sf::Vector2f p_vec) {
  return sf::Vector2f{std::round(p_vec.x), std::round(p_vec.y)};
}

sf::FloatRect merge_rects(sf::FloatRect rect, sf::FloatRect p_rect) {
  auto rect_pos = rect.getPosition();
  auto rect_size = rect.getSize();

  auto p_rect_pos = p_rect.getPosition();
  auto p_rect_size = p_rect.getSize();

  sf::Vector2f merged_rect_position = {std::min(p_rect_pos.x, rect_pos.x),
                                       std::min(p_rect_pos.y, rect_pos.y)};

  sf::Vector2f merged_rect_size = {
      std::max(p_rect_pos.x + p_rect_size.x, rect_pos.x + rect_size.x) -
          merged_rect_position.x,
      std::max(p_rect_pos.y + p_rect_size.y, rect_pos.y + rect_size.y) -
          merged_rect_position.y};

  return sf::FloatRect(merged_rect_position, merged_rect_size);
}

float distance_between(sf::Vector2f p1, sf::Vector2f p2) {
  auto x_dt = (p2.x - p1.x);
  auto y_dt = (p2.y - p1.y);

  return sqrt((x_dt * x_dt) + (y_dt * y_dt));
}

sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &end,
                  float lerpValue) {
  return (1.0f - lerpValue) * start + lerpValue * end;
}

float angle_between_points(sf::Vector2f p_from, sf::Vector2f p_to) {
  auto diff = (p_to - p_from);
  return atan2(diff.y, diff.x) * 180.f / 3.1415F;
}

float str_to_float(const std::string &str, float default_value) {
  try {
    return static_cast<float>(std::stoi(str));
  } catch (std::invalid_argument const &ex) {
    return default_value;
  }

  return default_value;
}

float range_to_percent(float p_min, float p_max, float p_value) {
  return 100 * (p_value - p_min) / (p_max - p_min);
}

} // namespace MATH_UTILITIES

namespace BLOCK_UTILITIES {
bool is_forever_block(const Block &block) {
  // The first child of "Forever" Block has text "Forever".
  return std::any_of(block.childrens.begin(), block.childrens.end(),
                     [](std::shared_ptr<NODEBaseClass> p_block) {
                       return p_block->get_text() == "Forever";
                     });
}

bool is_input_block(const Block &block) {
  return std::any_of(block.childrens.begin(), block.childrens.end(),
                     [](std::shared_ptr<NODEBaseClass> p_block) {
                       return p_block->get_text() == "When It's Clicked";
                     });
}

bool is_program_starts_block(const Block &block) {
  return std::any_of(block.childrens.begin(), block.childrens.end(),
                     [](std::shared_ptr<NODEBaseClass> p_block) {
                       return p_block->get_text() == "When Program Starts";
                     });
}

} // namespace BLOCK_UTILITIES
