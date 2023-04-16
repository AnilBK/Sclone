#include "Utils.hpp"
#include <algorithm>

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
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == ch) {
      s.erase(s.begin() + i);
      break;
    }
  }
}

void remove_last_occurence(std::string &s, char ch) {
  for (int i = s.length() - 1; i >= 0; i--) {
    if (s[i] == ch) {
      s.erase(s.begin() + i);
      break;
    }
  }
}
} // namespace STRING_UTILITIES

namespace MATH_UTILITIES {
sf::FloatRect merge_rects(sf::FloatRect rect, sf::FloatRect p_rect) {
  auto rect_pos = sf::Vector2f(rect.left, rect.top);
  auto rect_size = sf::Vector2f(rect.width, rect.height);

  auto p_rect_pos = sf::Vector2f(p_rect.left, p_rect.top);
  auto p_rect_size = sf::Vector2f(p_rect.width, p_rect.height);

  sf::Vector2f merged_rect_position = {std::min(p_rect_pos.x, rect_pos.x),
                                       std::min(p_rect_pos.y, rect_pos.y)};

  sf::Vector2f merged_rect_size = {
      std::max(p_rect_pos.x + p_rect_size.x, rect_pos.x + rect_size.x) -
          merged_rect_position.x,
      std::max(p_rect_pos.y + p_rect_size.y, rect_pos.y + rect_size.y) -
          merged_rect_position.y};

  return sf::FloatRect(merged_rect_position, merged_rect_size);
}

sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &end,
                  float lerpValue) {
  return (1.0f - lerpValue) * start + lerpValue * end;
}

float angle_between_points(sf::Vector2f p_from, sf::Vector2f p_to) {
  auto diff = (p_to - p_from);
  return atan2(diff.y, diff.x) * 180.f / 3.1415F;
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
