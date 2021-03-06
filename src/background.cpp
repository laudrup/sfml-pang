#include "background.h"

Background::Background() {
  sf::Image bg_image;
  bg_image.loadFromFile("data/background.png");
  sf::Texture texture;
  for (int i = 8; i < 25 * (208 + 8); i += 216) {
    texture.loadFromImage(bg_image, sf::IntRect(8, i, 384, 208));
    textures_.push_back(texture);
    texture.loadFromImage(bg_image, sf::IntRect(400, i, 384, 208));
    textures_.push_back(texture);
  }
  sprite_.setTexture(textures_[1]);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
}
