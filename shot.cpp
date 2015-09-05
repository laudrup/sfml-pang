#include "shot.h"

#include <iostream>

namespace res = thor::Resources;

Shot::Shot(sf::Vector2f pos, const sf::IntRect& area, float initial_height,
           thor::ResourceHolder<sf::Texture, std::string>& resources)
  : area_(area)
{
  setTexture(&resources.acquire("shot",
                                res::fromFile<sf::Texture>("gfx/shot.png"),
                                res::Reuse));
  setScale(2.f, area.height / static_cast<float>(textures_.begin()->height));
  setSize({static_cast<float>(textures_.begin()->width), initial_height / getScale().y * 2});
  setOrigin(0.f, 0.f);
  setPosition(pos.x, pos.y - getGlobalBounds().height / 2.f);
  for (const auto& rect : textures_)
  {
    shot_anim_.addFrame(1.f, rect);
  }
  anim_.addAnimation("shot", shot_anim_, sf::seconds(0.3f));
  anim_.playAnimation("shot", true);
}

sf::FloatRect Shot::bounds() const
{
  return sf::FloatRect(getPosition().x - getOrigin().x,
                       getPosition().y - getOrigin().y,
                       getGlobalBounds().width,
                       getGlobalBounds().height);
}

void Shot::update(sf::Time delta_time)
{
  move({speed_.x, speed_.y * -delta_time.asSeconds()});
  setSize({getSize().x, getSize().y + speed_.y * delta_time.asSeconds() / getScale().y});

  anim_.update(delta_time);
  anim_.animate(*this);

  auto rect = getTextureRect();
  rect.height = getSize().y;
  setTextureRect(rect);
}
