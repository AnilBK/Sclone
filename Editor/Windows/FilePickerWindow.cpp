#include "FilePickerWindow.hpp"
#include <cstddef>
#include <iostream>

FilePickerWindow::FilePickerWindow() {}

void FilePickerWindow::_fetch_file_names() {
  file_names = FileSystemUtils::get_image_file_names_list();
}

void FilePickerWindow::_generate_thumbnail_objects() {
  if (file_names.empty()) {
    return;
  }

  textures.reserve(file_names.size());
  thumbnails.reserve(file_names.size());

  constexpr float x_spacing = 32, y_spacing = 32;
  const sf::Vector2f thumbnail_size = sf::Vector2f(128, 128);

  sf::Vector2f pos = sf::Vector2f(50, 50);

  for (const auto &file : file_names) {
    std::shared_ptr<sf::Texture> texture(new sf::Texture());
    if (!texture.get()->loadFromFile(file)) {
      continue;
    }

    texture.get()->setSmooth(true);
    textures.emplace_back(texture);

    sf::Sprite sprite;
    sprite.setPosition(pos);
    sprite.setTexture(*textures.back().get());

    sprite.setScale(thumbnail_size.x / sprite.getLocalBounds().width,
                    thumbnail_size.y / sprite.getLocalBounds().height);

    sf::RoundedRectangleShape roundedRectangle;
    roundedRectangle.setSize(thumbnail_size + sf::Vector2f(10, 10));
    roundedRectangle.setFillColor(sf::Color(209, 250, 229));
    roundedRectangle.setCornersRadius(5);
    roundedRectangle.setOutlineThickness(5);
    roundedRectangle.setPosition(pos - sf::Vector2f(5, 5));
    roundedRectangle.setCornerPointCount(4);

    Thumbnail m_thumbnail;
    m_thumbnail.sprite = sprite;
    m_thumbnail.sprite_bg_rect = roundedRectangle;

    thumbnails.push_back(m_thumbnail);

    pos.x += thumbnail_size.x + x_spacing;

    if (pos.x + thumbnail_size.x > 800) {
      pos.x = 50;
      pos.y += thumbnail_size.y + y_spacing;
    }
  }
}

void FilePickerWindow::instantiate() {
  selected_file = "";
  file_names.clear();
  textures.clear();
  thumbnails.clear();

  is_active = true;
  _fetch_file_names();
  _generate_thumbnail_objects();

  file_picker_window.create(sf::VideoMode(800, 600), "Pick an image...");
  file_picker_window.setVisible(true);
}

void FilePickerWindow::_handle_inputs() {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(file_picker_window);

    std::size_t i = 0;
    for (const auto &thumbnail : thumbnails) {
      auto rect = thumbnail.sprite.getGlobalBounds();
      if (rect.contains(static_cast<sf::Vector2f>(mouse_pos))) {
        selected_file = file_names.at(i);

        is_active = false;
        file_picker_window.close();

        if (file_selected_callback) {
          file_selected_callback(selected_file);
        }

        break;
      }
      i++;
    }
  }
}

void FilePickerWindow::MainLoop() {
  if (!is_active) {
    return;
  }

  if (!file_picker_window.hasFocus()) {
    file_picker_window.requestFocus();
  }

  _handle_inputs();

  file_picker_window.clear(sf::Color::White);
  Render();
  file_picker_window.display();
}

void FilePickerWindow::Render() {
  for (const auto &thumbnail : thumbnails) {
    file_picker_window.draw(thumbnail.sprite_bg_rect);
    file_picker_window.draw(thumbnail.sprite);
  }
}
