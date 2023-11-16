#ifndef FILEPICKERWINDOWWIN32_HPP
#define FILEPICKERWINDOWWIN32_HPP

#include <functional>
#include <string>
#include <windows.h>

// File Picker Implementation that uses the Win32 file picker.
class FilePickerWindowWin32 {
private:
  // Win32 File Dialog setup
  OPENFILENAME ofn;
  char szFileName[MAX_PATH] = "";
  char currentDirectory[MAX_PATH];

public:
  std::function<void(std::string file_name)> file_selected_callback;

  FilePickerWindowWin32();

  void instantiate();
};

#endif // FILEPICKERWINDOWWIN32_HPP
