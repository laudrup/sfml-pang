#include "ball.h"

#include <cassert>
#include <iostream>

namespace res = thor::Resources;

Ball::Ball(Type type,
           sf::Color color,
           sf::Vector2f position,
           Direction direction,
           const sf::FloatRect& area,
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

  if (new_pos.x - getRadius() <= area_.left) { // left edge
    velocity_.x *= -1;
    direction_ = Direction::West;
    new_pos.x = area_.left + getRadius();
  } else if (new_pos.x + getRadius() >= area_.width) { // right edge
    velocity_.x *= -1;
    direction_ = Direction::East;
    new_pos.x = area_.width - getRadius();
  } else if (new_pos.y - getRadius() < area_.top) { // top of window
    velocity_.y *= -1;
    new_pos.y = area_.top + getRadius();
  } else if (new_pos.y + getRadius() >= area_.height) { // bottom of window
    velocity_.y = gravity.y;
    velocity_.y *= -1;
    new_pos.y = area_.height - getRadius();
  }
  setPosition(new_pos);
}

std::pair<Ball, Ball> Ball::split() const {
  Type type{};
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
