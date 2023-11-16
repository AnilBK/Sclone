#ifndef FILESYSTEMUTILS_HPP
#define FILESYSTEMUTILS_HPP

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

namespace FileSystemUtils {
// E:\Sclone 2.0\Debug\1\cat.png -> cat.png
std::string _image_file_name_from_full_path(const std::string &p_path);

std::vector<std::string> get_image_file_names_list();
} // namespace FileSystemUtils

#endif // FILESYSTEMUTILS_HPP