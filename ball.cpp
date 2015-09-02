#include "ball.h"

#include <iostream>
#include <cassert>

namespace res = thor::Resources;

Ball::Ball(Type type, sf::Color color, sf::Vector2f position,
           Direction direction, const sf::IntRect& area,
           thor::ResourceHolder<sf::Texture, std::string>* resources)
  : type_(type),
    velocity_(10.f * static_cast<int>(direction), -20.f), area_(area),
    texture_(&resources->acquire("ball", res::fromFile<sf::Texture>("gfx/ball.png"),
                                 res::Reuse)),
    resources_(resources)
{
  switch (type)
  {
  case Type::Large:
    setRadius(50.f);
    mass_ = 15.f;
    break;
  case Type::Medium:
    setRadius(40.f);
    mass_ = 20.f;
    break;
  case Type::Small:
    setRadius(25.f);
    mass_ = 25.f;
    break;
  case Type::Tiny:
    setRadius(12.f);
    mass_ = 50.f;
    break;
  }
  setFillColor(color);
  setOrigin(getRadius(), getRadius());
  setPosition(position);
  texture_->setSmooth(true);
  setTexture(texture_);
}

void Ball::update(const sf::Vector2f gravity, const sf::Time delta_time)
{
  velocity_ = velocity_ + (gravity * delta_time.asSeconds())
    * (mass_ *delta_time.asSeconds());
  sf::Vector2f new_pos = getPosition() + velocity_;

  if (new_pos.x - getRadius() < area_.left) // left edge
  {
    velocity_.x *= -1;
    new_pos.x = area_.left + getRadius();
  }
  else if (new_pos.x + getRadius() >= area_.width + area_.left) // right edge
  {
    velocity_.x *= -1;
    new_pos.x = area_.width - getRadius();
  }
  else if (new_pos.y - getRadius() < area_.top) // top of window
  {
    velocity_.y *= -1;
    new_pos.y = area_.top + getRadius();
  }
  else if (new_pos.y + getRadius() >= area_.height + area_.top) // bottom of window
  {
    velocity_.y = gravity.y;
    velocity_.y *= -1;
    new_pos.y = area_.height + area_.top - getRadius();
  }
  setPosition(new_pos);
}

sf::FloatRect Ball::bounds() const
{
  return sf::FloatRect(getPosition().x - getOrigin().x,
                       getPosition().y - getOrigin().y,
                       getGlobalBounds().width,
                       getGlobalBounds().height);
}


std::pair<Ball, Ball> Ball::split() const
{
  Type type;
  switch(type_)
  {
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

  return
  {
    Ball(type, getFillColor(), sf::Vector2f(pos.x, pos.y),
         Ball::Direction::West, area_, resources_),
    Ball(type, getFillColor(), sf::Vector2f(pos.x, pos.y),
         Ball::Direction::East, area_, resources_)
  };
}
