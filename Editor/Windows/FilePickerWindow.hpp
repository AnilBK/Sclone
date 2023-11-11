#ifndef FILEPICKERWINDOW_HPP
#define FILEPICKERWINDOW_HPP

#include "../../FileSystemUtils.hpp"
#include "../../thirdparty/RoundedRectangleShape.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

inline sf::RenderWindow file_picker_window;

class FilePickerWindow {

private:
  struct Thumbnail {
    sf::Sprite sprite;
    sf::RoundedRectangleShape sprite_bg_rect;
  };

  std::vector<std::string> file_names;
  std::vector<std::shared_ptr<sf::Texture>> textures;
  std::vector<Thumbnail> thumbnails;

  std::string selected_file;

  bool is_active = false;

  void _fetch_file_names();
  void _generate_thumbnail_objects();
  void _handle_inputs();

public:
  std::function<void(std::string file_name)> file_selected_callback;

  FilePickerWindow();

  void instantiate();

  void Render();

  void MainLoop();
};

#endif // FILEPICKERWINDOW_HPP
