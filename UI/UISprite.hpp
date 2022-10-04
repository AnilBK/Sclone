#ifndef UI_SPRITE
#define UI_SPRITE

#include "UIBaseClass.hpp"

class UISprite : public UIBaseClass {
private:
  sf::Texture texture;

public:
  // TODO:Make this private and add helper members instead.
  sf::Sprite sprite;

  UISprite(std::string file_name, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f)) {
    texture.loadFromFile(file_name);
    texture.setSmooth(true);

    sprite.setTexture(texture);
    // sf::FloatRect size = sprite.getGlobalBounds();
    // sprite.setOrigin(size.width / 2.0f, size.height / 2.0f);
    sprite.setPosition(pos);
  }

  sf::Vector2f getPosition() override;
  void setPosition(sf::Vector2f pos) override;
  sf::Vector2f rect_size() override;

  void RenderDebug() override;
  void Render() override;

  void handle_inputs(sf::Event event) override {}
};
#endif // UI_SPRITE
