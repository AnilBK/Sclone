#include "FileSystemUtils.hpp"
#include <iostream>

namespace FileSystemUtils {
std::vector<std::string> get_image_file_names_list() {
  std::vector<std::string> file_names;

  namespace fs = boost::filesystem;

  try {
    fs::path currentPath = fs::current_path();

    // Iterate over the contents of the current directory.
    for (const auto &entry : fs::directory_iterator(currentPath)) {
      auto &file = entry.path();
      if (fs::is_regular_file(file) &&
          (file.extension() == ".jpg" || file.extension() == ".png")) {
        auto name = file.filename().string();
        file_names.push_back(name);
      }
    }
  } catch (const fs::filesystem_error &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }

  return file_names;
}
} // namespace FileSystemUtils