#include <string>
#include <vector>

// This namespace provides set of utilities for handling fString(FormatString)
// operations.
namespace fStrings {

// fStrings also called FormatStrings, are the strings with variables embedded
// in them. For e.g: "2 + 2 = 4" isn't a fString because we don't have any
// variables embedded in them.
// But "{x} + {y} = {sum}" is a fString.
// We use {var_name} to reference a variable named 'var_name'.

struct fString_variable {
  /// @brief The variable name extracted from the given string.
  std::string var_name;
  /// @brief Position of the var_name in the input string.
  std::size_t left_brace_start_pos = 0, right_brace_start_pos = 0;
};

bool is_fString(const std::string &str);

/// @brief It takes an input string like "Score: {score} Health: {health}" and
/// returns fString_variable that contains the variable name e.g 'score' and
/// position of 'score' in the input string.
/// For e.g: _extract_variables("Score:{score}Health:{health}") returns
/// fString_variable{"score",6,12}, fString_variable{"health",20,27}
/// @param str The input string.
/// @return Returns a list of fString Variables.
std::vector<fString_variable> _extract_variables(const std::string &str);

/// @brief _apply_transformation() transforms a String: "Score of {player_name}
/// = {score}." to  "Score of _##SPRITE_NAME##_player_name =
/// _##SPRITE_NAME##_score."
/// @param original_string The input string
void _apply_transformation(std::string &original_string);

/// @brief It takes a fString and apply all the required transformations so that
/// it is ready for CodeGen. It takes a string with variables and mangles it
/// properly so that it works in the cpp backend.
/// E.g: Score:{score}Health:{health} becomes
/// "Score:" + std::to_string(_##SPRITE_NAME##_score) + "Health:" +
/// std::to_string(_##SPRITE_NAME##_health)
/// + ""
/// @param code The input code to be transformed.
/// @return String containing the code which ready to be written to cpp.
std::string formatted_string_to_code(const std::string &code);

} // namespace fStrings