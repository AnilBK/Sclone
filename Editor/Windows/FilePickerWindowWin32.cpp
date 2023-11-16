#include "FilePickerWindowWin32.hpp"
#include <iostream>

FilePickerWindowWin32::FilePickerWindowWin32() {
  // This Win32 implementation was provided by ChatGPT, so not sure, if it's
  // correct, works though.
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter =
      "Image Files (*.jpg;*.png)\0*.jpg;*.png\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = szFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = "jpg";

  // Set the initial directory to the current directory
  GetCurrentDirectory(MAX_PATH, currentDirectory);
  ofn.lpstrInitialDir = currentDirectory;
}

void FilePickerWindowWin32::instantiate() {
  // std::cout << "Current Directory : " << currentDirectory << "\n";
  if (GetOpenFileName(&ofn)) {
    if (file_selected_callback) {
      file_selected_callback(szFileName);
    }
  }
}
