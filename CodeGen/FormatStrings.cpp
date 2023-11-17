#include "FormatStrings.hpp"

namespace fStrings {

bool is_fString(const std::string &str) {
  // var_name -> Not A fString.
  // {var_name} -> A fString.
  auto left_pos = str.find('{');

  auto found_left_brace = (left_pos != std::string::npos);
  if (!found_left_brace) {
    return false;
  }

  // '}' should follow '{'.
  auto right_pos = str.find('}', left_pos);

  auto found_right_brace = (right_pos != std::string::npos);
  return found_right_brace;
}

std::vector<fString_variable> _extract_variables(const std::string &str) {
  std::vector<fString_variable> variables;

  fString_variable temp_string;

  bool in_braces = false;
  std::string current_variable;

  // TODO: Handle spaces in between_braces before the variable_name starts.
  // E.g : { var_name}

  std::size_t i = 0;
  for (char ch : str) {
    if (ch == '{') {
      temp_string.left_brace_start_pos = i;
      in_braces = true;
    } else if (ch == '}') {
      in_braces = false;
      temp_string.right_brace_start_pos = i;
      temp_string.var_name = current_variable;
      variables.push_back(temp_string);
      current_variable = "";
    } else if (in_braces) {
      current_variable += ch;
    }
    i++;
  }

  return variables;
}

void _apply_transformation(std::string &original_string) {
  std::vector<fString_variable> fStrings = _extract_variables(original_string);

  auto _var_name_to_string = [](const std::string &var_name) {
    // User Defined Variable name: var_name
    // To internal variable name:  std::to_string(_##SPRITE_NAME##_var_name)
    return "std::to_string(_##SPRITE_NAME##_" + var_name + ")";
  };

  // We have to replace strings at given positions. We just have the positions
  // of where to replace the strings. If we start replacing from the start, then
  // our original positions now refer to different range of characters. So we
  // iterate from the last.
  for (auto i = fStrings.rbegin(); i != fStrings.rend(); i++) {
    auto temp_fString = *i;
    // The "  is added at the start & end to terminate already started string
    // sequences.
    auto mangled_string = std::string("\" + ") +
                          _var_name_to_string(temp_fString.var_name) +
                          std::string(" + \"");
    auto len = temp_fString.right_brace_start_pos -
               temp_fString.left_brace_start_pos + 1;
    original_string.erase(temp_fString.left_brace_start_pos, len);
    original_string.insert(temp_fString.left_brace_start_pos, mangled_string);
  }

  original_string.insert(0, "\"");
  original_string.append("\"");
}

std::string formatted_string_to_code(const std::string &code) {
  auto string = code;
  _apply_transformation(string);
  return string;
}

} // namespace fStrings
