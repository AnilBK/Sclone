#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include "../Editor/Editor.hpp"
#include "../Globals.hpp"
#include <algorithm>
#include <fstream>
#include <set>
#include <string>
#include <vector>

class Editor;
class EditorSprite;

class CodeGenerator {
private:
  // The CodeGenerator Object takes Reference to the editor so that it has
  // access to all the user added sprites and scripts.
  Editor &editor_ref;

  void _substitute_sprite_name(std::string &code,
                               const std::string &sprite_name);
  std::string _construct_sprite_code(const EditorSprite &spr);
  std::string _render_sprite_code(const std::string &sprite_name);
  std::string _get_update_player_velocity_code();

  std::string construct_code();
  std::string render_all_sprites_code();
  std::string main_loop_code();
  std::string input_code();
  std::string when_program_starts_code();

  std::set<std::string> get_all_modules_required();

public:
  CodeGenerator(Editor &editor) : editor_ref(editor) {}

  void generate_code();
};

namespace Preprocessor {
// The Preprocessor:
// It looks for modules in the 'template.cpp'.
// If the module isn't required for the final generated code, then they are
// stripped away from the final generated code.
void pre_process(std::string &code,
                 const std::set<std::string> &required_modules);

} // namespace Preprocessor

#endif // CODE_GENERATOR_HPP
