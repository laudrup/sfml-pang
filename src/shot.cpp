#include "shot.h"

namespace res = thor::Resources;

Shot::Shot(sf::Vector2f pos,
           const sf::FloatRect& area,
           float initial_height,
           thor::ResourceHolder<sf::Texture, std::string>& resources)
    : area_(area) {
  setTexture(&resources.acquire("shot", res::fromFile<sf::Texture>("data/shot.png"), res::Reuse));
  setSize({static_cast<float>(textures_.begin()->width), initial_height});
  setPosition(pos.x, pos.y - initial_height);
  for (const auto& rect : textures_) {
    shot_anim_.addFrame(1.f, rect);
  }
  anim_.addAnimation("shot", shot_anim_, sf::seconds(0.3f));
  anim_.playAnimation("shot", true);
}

void Shot::update(sf::Time delta_time) {
  move({speed_.x, speed_.y * -delta_time.asSeconds()});
  setSize({getSize().x, getSize().y + speed_.y * delta_time.asSeconds()});

  anim_.update(delta_time);
  anim_.animate(*this);

  auto rect = getTextureRect();
  rect.height = static_cast<int>(getSize().y);
  setTextureRect(rect);
}
