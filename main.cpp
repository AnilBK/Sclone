#include <SFML/Graphics.hpp>
#include <iostream>

int height = 800;
int width = 800;

sf::Font font;
sf::Text draw_text_label;

sf::RenderWindow window;

// Final Block Output:
// VVVVVVVVVVVVVVVVVVVVVVVVVV
// Say "Text" for "2" seconds.
// The Generator API.
//   Label("Say")
//     LineInputAttachField(text_variable)
//       Label("for")
//         LineInputAttachField(time_variable)
//           Label("seconds").

// With this new API.
// Print "String" becomes
//   Label("Print")
//     LineInputAttachField(Str)

#define ERR_FAIL_COND_CRASH(m_cond, m_fail_message)                            \
  if (!m_cond) {                                                               \
    std::cout << m_fail_message << "\n";                                       \
    exit(1);                                                                   \
  }

void init_global_font_and_label() {
  ERR_FAIL_COND_CRASH(font.loadFromFile("alaska.ttf"), "Error Loading Font.");
  draw_text_label.setFont(font);
  draw_text_label.setCharacterSize(32);
  draw_text_label.setPosition(sf::Vector2f(20, 20));
  draw_text_label.setFillColor(sf::Color::Black);
}

void draw_text(const std::string &str, const sf::Vector2f p_position) {
  draw_text_label.setString(str);
  draw_text_label.setPosition(p_position);
  window.draw(draw_text_label);
}

sf::Vector2f get_text_rect_size(const std::string &str) {
  draw_text_label.setString(str);
  return {draw_text_label.getGlobalBounds().width,
          draw_text_label.getGlobalBounds().height};
}

sf::Vector2f line_input_field_rect_size() { return {25, 35}; }

void draw_line_input_attach_field(const sf::Vector2f p_position) {
  sf::RectangleShape r;
  r.setPosition(p_position);
  r.setSize(line_input_field_rect_size());
  r.setFillColor(sf::Color::Red);
  window.draw(r);
}

int main() {

  init_global_font_and_label();

  height = sf::VideoMode::getDesktopMode().height;
  width = sf::VideoMode::getDesktopMode().width;

  window.create(sf::VideoMode(width, height), "SClone V2");

  enum NODE_TYPE { LABEL, LINE_INPUT_ATTACH_FIELD };

  struct NODE {
    std::string text;
    NODE_TYPE type;
  };

  std::vector<NODE> childrens;
  //   Label("Say")
  //     LineInputAttachField(text_variable)
  //       Label("for")
  //         LineInputAttachField(time_variable)
  //           Label("seconds").
  childrens.push_back({"Say", NODE_TYPE::LABEL});
  childrens.push_back({"", NODE_TYPE::LINE_INPUT_ATTACH_FIELD});
  childrens.push_back({"for", NODE_TYPE::LABEL});
  childrens.push_back({"", NODE_TYPE::LINE_INPUT_ATTACH_FIELD});
  childrens.push_back({"seconds", NODE_TYPE::LABEL});

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color(0, 255, 204));

    // draw_text("Hello World", {200.0f, 200.0f});
    // draw_line_input_attach_field({250.0f, 200.0f});

    sf::Vector2f pos{200.0f, 300.0f};
    float spacing = 5;

    float padding_left = 5, padding_right = 5;
    float padding_up = 5, padding_down = 5;

    sf::Vector2f block_size{0.0f, 45.0f};
    sf::Vector2f block_position{pos.x - padding_left, pos.y - padding_up};

    // TODO:Update only if dirty.
    // Recompute Rect() {
    for (auto child : childrens) {
      if (child.type == NODE_TYPE::LABEL) {
        block_size.x += get_text_rect_size(child.text).x;
        block_size.x += spacing;
      }

      if (child.type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
        block_size.x += line_input_field_rect_size().x;
        block_size.x += spacing;
      }
    }

    // Margins
    // Account for the block position decreased during padding.
    block_size.x += padding_left + padding_right;
    block_size.y += padding_up + padding_down;

    sf::RectangleShape block;
    block.setPosition(block_position);
    block.setSize(block_size);
    block.setFillColor(sf::Color::Green);
    window.draw(block);

    for (auto child : childrens) {
      if (child.type == NODE_TYPE::LABEL) {
        draw_text(child.text, pos);
        pos.x += get_text_rect_size(child.text).x;
        pos.x += spacing;
      }

      if (child.type == NODE_TYPE::LINE_INPUT_ATTACH_FIELD) {
        draw_line_input_attach_field(pos);
        pos.x += line_input_field_rect_size().x;
        pos.x += spacing;
      }
    }

    window.display();
  }

  return 0;
}
