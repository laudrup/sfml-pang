#include "player.h"

namespace res = thor::Resources;

Player::Player(sf::Vector2f pos, const sf::FloatRect& area, thor::ResourceHolder<sf::Texture, std::string>& resources)
    : direction_(Direction::Stopped)
    , state_(State::Alive)
    , area_(area)
    , resources_(resources)
    , death_anim_velocity_(-1.5f, -5.f) {
  sf::Image img;
  if (!img.loadFromFile("data/player.png")) {
    abort();
  }
  setTexture(resources.acquire("player", res::fromImage<sf::Texture>(img)));
  setTextureRect(*walk_textures_.begin());
  setOrigin(getLocalBounds().width / 2, getLocalBounds().height);
  setPosition(pos.x, pos.y);
  for (const auto& rect : walk_textures_) {
    walk_anim_.addFrame(1.f, rect);
  }
  anim_.addAnimation("walk", walk_anim_, sf::seconds(0.6f));

  for (const auto& rect : shoot_textures_) {
    shoot_anim_.addFrame(1.f, rect);
  }
  anim_.addAnimation("shoot", shoot_anim_, sf::seconds(0.3f));
}

void Player::removeShot() {
  // TODO: Handle more shots
  shots_.clear();
}

void Player::die(Ball::Direction direction) {
  state_ = State::Dying;
  switch (direction) {
  case Ball::Direction::East:
    setTextureRect(death_textures_[0]);
    death_anim_velocity_ = {1.5f, -5.f};
    break;
  case Ball::Direction::West:
    setTextureRect(death_textures_[1]);
    death_anim_velocity_ = {-1.5f, -5.f};
    break;
  }
}

bool Player::handleEvent(sf::Event event) {
  if (state_ != State::Alive) {
    return false;
  }
  switch (event.type) {
  case sf::Event::KeyPressed:
    if (event.key.code == sf::Keyboard::Left) {
      direction_ = Direction::Left;
      if (!anim_.isPlayingAnimation() || anim_.getPlayingAnimation() != "walk")
        anim_.playAnimation("walk", true);
    } else if (event.key.code == sf::Keyboard::Right) {
      direction_ = Direction::Right;
      if (!anim_.isPlayingAnimation() || anim_.getPlayingAnimation() != "walk")
        anim_.playAnimation("walk", true);
    } else if (event.key.code == sf::Keyboard::Space && shots_.empty()) {
      direction_ = Direction::Stopped;
      if (!anim_.isPlayingAnimation() || anim_.getPlayingAnimation() != "shoot")
        anim_.playAnimation("shoot");
      shots_.push_back(std::make_shared<Shot>(
          sf::Vector2f(getPosition().x, getPosition().y), area_, getLocalBounds().height, resources_));
    }
    return true;

  case sf::Event::KeyReleased:
    if (event.key.code == sf::Keyboard::Left && direction_ == Direction::Left) {
      direction_ = Direction::Stopped;
      anim_.stopAnimation();
    } else if (event.key.code == sf::Keyboard::Right && direction_ == Direction::Right) {
      direction_ = Direction::Stopped;
      anim_.stopAnimation();
    } else if (event.key.code == sf::Keyboard::Space) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction_ = Direction::Left;
        anim_.playAnimation("walk", true);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction_ = Direction::Right;
        anim_.playAnimation("walk", true);
      }
    }
    return true;

  default:
    break;
  }
  return false;
}

void Player::update(sf::Time delta_time) {
  if (state_ == State::Dying) {
    if (getPosition().x + getGlobalBounds().width / 2 <= 0 ||
        getPosition().x - getGlobalBounds().width / 2 >= area_.width ||
        getPosition().y <= 0 ||
        getPosition().y - getGlobalBounds().height >= area_.height) {
      state_ = State::Dead;
    }

    const sf::Vector2f gravity(0.f, 5.f);
    death_anim_velocity_ = death_anim_velocity_ + (gravity * delta_time.asSeconds()) * (20.f * delta_time.asSeconds());
    sf::Vector2f new_pos = getPosition() + death_anim_velocity_;

    setPosition(new_pos);
    return;
  }
  for (auto& shot : shots_) {
    shot->update(delta_time);
  }
  shots_.erase(std::remove_if(shots_.begin(),
                              shots_.end(),
                              [this](auto shot) {
                                return shot->getGlobalBounds().top <= area_.top;
                              }),
               shots_.end());
  if (direction_ == Direction::Left) {
    setScale(-1.f, 1.f);
    move({-speed_.x * delta_time.asSeconds(), 0.f});
    if (getPosition().x - getLocalBounds().width / 2 < area_.left) {
      setPosition(area_.left + getLocalBounds().width / 2, getPosition().y);
    }
  } else if (direction_ == Direction::Right) {
    setScale(1.f, 1.f);
    move({speed_.x * delta_time.asSeconds(), 0.f});
    if (getPosition().x > area_.width - getLocalBounds().width / 2) {
      setPosition(area_.width - getLocalBounds().width / 2, getPosition().y);
    }
  } else if (direction_ == Direction::Stopped) {
  }
  anim_.update(delta_time);
  anim_.animate(*this);
}
