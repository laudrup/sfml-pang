#include "background.h"

#include <iostream>

namespace res = thor::Resources;

Background::Background(const sf::Vector2u& area,
                       thor::ResourceHolder<sf::Texture, std::string>& resources)
{
  sprite_.setTexture(resources.acquire("background",
                                       res::fromFile<sf::Texture>("gfx/background.png")));
  for (int i = 8; i < 25 * (208 + 8); i += 216)
  {
    textures_.push_back(sf::IntRect(8, i, 384, 208));
    textures_.push_back(sf::IntRect(400, i, 384, 208));
  }
  sprite_.setTextureRect(textures_[0]);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(sprite_, states);
}
