#include "CodeGen/CodeGenerator.hpp"
#include "Editor/Editor.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SHOW_FPS
#define SHOW_MOUSE_POS

unsigned int height = 800;
unsigned int width = 800;

sf::Vector2i mouse_position;

// Externs from globals.hpp
sf::Font font;
sf::Text draw_text_label;
sf::RenderWindow window;

#ifdef SHOW_FPS
float get_fps() {
  static sf::Clock clock;
  static sf::Time last_time = clock.getElapsedTime();
  sf::Time current_time = clock.getElapsedTime();
  float fps = 1.f / (current_time.asSeconds() - last_time.asSeconds());
  last_time = current_time;

  return fps;
}
#endif

int main() {
  init_global_font_and_label();

  sf::Font alaska;
  ERR_FAIL_COND_CRASH(!alaska.loadFromFile("alaska.ttf"),
                      "Error Loading Font \"alaska\".");

  height = sf::VideoMode::getDesktopMode().height;
  width = sf::VideoMode::getDesktopMode().width;

  window.create(sf::VideoMode(width, height), "SClone V2");

#ifdef SHOW_FPS
  sf::Text show_fps_btn;
  show_fps_btn.setString("FPS:");
  show_fps_btn.setFont(font);
  show_fps_btn.setPosition(sf::Vector2f(5, 40));
  show_fps_btn.setFillColor(sf::Color::Black);
#endif

#ifdef SHOW_MOUSE_POS
  sf::Text show_mouse_pos_text;
  show_mouse_pos_text.setString("");
  show_mouse_pos_text.setFont(font);
  show_mouse_pos_text.setPosition(sf::Vector2f(5, 5));
  show_mouse_pos_text.setFillColor(sf::Color::Black);
#endif

  const auto window_clear_color = sf::Color(153, 195, 180);

  Editor editor;

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyReleased &&
           event.key.code == sf::Keyboard::Escape)) {
        window.close();
      } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::LAlt) {
          CodeGenerator CodeGen(editor);
          CodeGen.generate_code();
        }
      }

      editor.handle_inputs(event);
    }

    mouse_position = sf::Mouse::getPosition(window);

    window.clear(window_clear_color);

    editor.Render();

#ifdef SHOW_FPS
    show_fps_btn.setString("FPS: " +
                           std::to_string(static_cast<int>(get_fps())));
    window.draw(show_fps_btn);
#endif

#ifdef SHOW_MOUSE_POS
    show_mouse_pos_text.setString("X: " + std::to_string(mouse_position.x) +
                                  " Y: " + std::to_string(mouse_position.y));
    window.draw(show_mouse_pos_text);
#endif

    window.display();
  }

  std::cout << "\nProgram Terminated.\n\n";

  return 0;
}
