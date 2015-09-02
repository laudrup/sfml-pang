#include "background.h"

#include <iostream>

namespace res = thor::Resources;

Background::Background(const sf::Vector2u& area,
                       thor::ResourceHolder<sf::Texture, std::string>& resources)
{
  sprite_.setTexture(resources.acquire("background",
                                       res::fromFile<sf::Texture>("gfx/background.png")));
  sprite_.setTextureRect(sf::IntRect(8, 8, 384, 208));
  sprite_.setScale(area.x / sprite_.getLocalBounds().width,
                   area.y / sprite_.getLocalBounds().height);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(sprite_, states);
}
