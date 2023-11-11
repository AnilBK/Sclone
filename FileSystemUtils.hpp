#ifndef FILESYSTEMUTILS_HPP
#define FILESYSTEMUTILS_HPP

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace FileSystemUtils {
std::vector<std::string> get_image_file_names_list();
}

#endif // FILESYSTEMUTILS_HPP