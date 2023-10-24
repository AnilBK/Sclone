#ifndef EDITOR_SPRITE_HPP
#define EDITOR_SPRITE_HPP

#include "../Nodes/Node.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Individual Sprites Added by the User.
class EditorSprite {
public:
  int id = -1; //-1 means Invalid.
  int layer = 0;
  std::string name;
  sf::Vector2f position;
  bool visibility;
  bool add_movement_script = false;
  std::string texture;
  sf::Sprite sprite;
  std::shared_ptr<Node> node;
};

#endif // EDITOR_SPRITE_HPP
