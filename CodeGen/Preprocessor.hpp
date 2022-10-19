#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include "../Globals.hpp"
#include <algorithm>
#include <set>
#include <string>
#include <vector>

namespace Preprocessor {
// The Preprocessor:
// It looks for modules in the 'template.cpp'.
// If the module isn't required for the final generated code, then they are
// stripped away from the final generated code.
void pre_process(std::string &code,
                 const std::set<std::string> &required_modules) {

  auto is_this_module_required = [&required_modules](const std::string &p_mod) {
    return required_modules.find(p_mod) != required_modules.end();
  };

  using index_position = unsigned int;
  using string_length = std::size_t;
  // Pair of 'position' and 'length' of the entire module code to strip away.
  std::vector<std::pair<index_position, string_length>> strip_pos_length_pair;

  // In 'template.cpp':
  // A module is defined as :
  // MODULE : "BUBBLE_TEXT" //

  const std::string module_def_str = "// MODULE : ";

  std::string::size_type pos = 0;
  // Find all the positions which starts with module_def_str.
  while ((pos = code.find(module_def_str, pos)) != std::string::npos) {
    pos += module_def_str.length();

    //  // MODULE : "BUBBLE_TEXT" //
    //              ^ pos
    //                           ^ module_name_end_pos

    // Module defination has // in the end.
    auto module_name_end_pos = code.find(" //", pos);

    // Get the name of module which is in between the strings  "// MODULE : "
    // and " //".
    auto module_name = code.substr(pos, module_name_end_pos - pos);

    // Module name obtained is : "BUBBLE_TEXT".
    // Remove the double quotes to get the actual string.
    remove_first_occurence(module_name, '\"');
    remove_last_occurence(module_name, '\"');

    bool keep_module = is_this_module_required(module_name);

    //  // MODULE : "BUBBLE_TEXT" //
    //              ^ pos
    //  ^ mod_start_pos
    auto mod_start_pos = pos - module_def_str.length();

    if (keep_module) {
      // The module should be in the final generated code.
      // However the module code contains all these preprocessor commands.
      // Strip the preprocessor commands away to make it look good, however the
      // actual module body is kept.

      //  // MODULE : "BUBBLE_TEXT" //
      //                          ^ module_name_end_pos
      //  ^ mod_start_pos             ^mod_def_end

      auto mod_def_end = module_name_end_pos + std::string(" //").length();
      auto mod_def_line_length = mod_def_end - mod_start_pos;

      strip_pos_length_pair.insert(strip_pos_length_pair.begin(),
                                   {mod_start_pos, mod_def_line_length});
    } else {
      // The required module shouldn't be in the final generated code.

      // A module defination ends with "// MODULE_END //".
      const std::string module_end_str = "// MODULE_END //";
      // // MODULE_END //".
      //                ^ module_def_end_pos
      auto module_def_end_pos =
          code.find(module_end_str, pos) + module_end_str.length();

      auto length_of_full_module_code = module_def_end_pos - mod_start_pos;

      // Add the 'position of start of the module code' and 'entire length of
      // the module code' into the beginning of a vector as a pair.
      // In this way, modules at the last of the template code appears in the
      // first of the vector, so we can erase the strings from behind without
      // messing the positions of strings.
      strip_pos_length_pair.insert(strip_pos_length_pair.begin(),
                                   {mod_start_pos, length_of_full_module_code});
    }
  }

  // From above, these are the module codes that shouldn't be in the final
  // generated code.
  // So strip all them.
  for (auto [pos, len] : strip_pos_length_pair) {
    code.erase(pos, len);
  }

  // This should be below the loop, so it doesn't mess up the position of
  // strings.
  replaceAll(code, "// MODULE_END //", "");
}

} // namespace Preprocessor

#endif // PREPROCESSOR_HPP
