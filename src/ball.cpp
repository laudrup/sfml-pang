#include "ball.h"

#include <cassert>
#include <iostream>

namespace res = thor::Resources;

Ball::Ball(Type type,
           sf::Color color,
           sf::Vector2f position,
           Direction direction,
           const sf::IntRect& area,
           thor::ResourceHolder<sf::Texture, std::string>* resources)
    : type_(type)
    , direction_(direction)
    , velocity_(1.5f * static_cast<float>(direction), -5.f)
    , area_(area)
    , texture_(&resources->acquire("ball", res::fromFile<sf::Texture>("gfx/ball.png"), res::Reuse))
    , resources_(resources) {
  switch (type) {
  case Type::Large:
    setRadius(12.5f);
    mass_ = 6.f;
    break;
  case Type::Medium:
    setRadius(10.f);
    mass_ = 12.f;
    break;
  case Type::Small:
    setRadius(6.5f);
    mass_ = 18.f;
    break;
  case Type::Tiny:
    setRadius(3.f);
    mass_ = 24.f;
    break;
  }
  setFillColor(color);
  setOrigin(getRadius(), getRadius());
  setPosition(position);
  texture_->setSmooth(true);
  setTexture(texture_);
}

void Ball::update(const sf::Time delta_time) {
  const sf::Vector2f gravity(0.f, 5.f);
  velocity_ = velocity_ + (gravity * delta_time.asSeconds()) * (mass_ * delta_time.asSeconds());
  sf::Vector2f new_pos = getPosition() + velocity_;

  if (new_pos.x - getRadius() <= 8) { // left edge
    velocity_.x *= -1;
    direction_ = static_cast<Direction>(static_cast<int>(direction_) * -1);
    new_pos.x = static_cast<float>(area_.left) + getRadius() + 8;
  } else if (new_pos.x + getRadius() >= static_cast<float>(area_.width - 8)) { // right edge
    velocity_.x *= -1;
    direction_ = static_cast<Direction>(static_cast<int>(direction_) * -1);
    new_pos.x = static_cast<float>(area_.width) - getRadius() - 8;
  } else if (new_pos.y - getRadius() < 8) { // top of window
    velocity_.y *= -1;
    new_pos.y = static_cast<float>(area_.top) + getRadius() + 8;
  } else if (new_pos.y + getRadius() >= static_cast<float>(area_.height) - 8) { // bottom of window
    velocity_.y = gravity.y;
    velocity_.y *= -1;
    new_pos.y = static_cast<float>(area_.height) - getRadius() - 8;
  }
  setPosition(new_pos);
}

sf::FloatRect Ball::bounds() const {
  return sf::FloatRect(getPosition().x - getOrigin().x,
                       getPosition().y - getOrigin().y,
                       getGlobalBounds().width,
                       getGlobalBounds().height);
}

std::pair<Ball, Ball> Ball::split() const {
  Type type;
  switch (type_) {
  case Ball::Type::Large:
    type = Type::Medium;
    break;
  case Ball::Type::Medium:
    type = Type::Small;
    break;
  case Ball::Type::Small:
    type = Type::Tiny;
    break;
  case Ball::Type::Tiny:
    abort();
  }
  const auto pos = getPosition();

  return {Ball(type, getFillColor(), sf::Vector2f(pos.x, pos.y), Ball::Direction::West, area_, resources_),
          Ball(type, getFillColor(), sf::Vector2f(pos.x, pos.y), Ball::Direction::East, area_, resources_)};
}
