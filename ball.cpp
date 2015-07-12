#include "ball.h"

#include <cassert>

Ball::Ball(sf::RenderWindow& window, sf::Vector2f pos, Type type, sf::Color color,
           Direction direction)
  : window_(window),
    pos_(pos),
    radius_(type_properties_.at(type).radius),
    velocity_(type_properties_.at(type).velocity),
    angle_(M_PI * 2 / 5),
    type_(type),
    color_(color)
{
  x_velocity_ = std::cos(angle_) * velocity_;
  y_velocity_ = -(std::sin(angle_) * velocity_);

  if(pos.x + radius_ * 2 >= window_.getSize().x)
  {
    pos_.x = window_.getSize().x - radius_ * 2;
  }
  assert(pos.y + radius_ * 2 <= window_.getSize().y);

  if (direction == Direction::West)
  {
    x_velocity_ = -x_velocity_;
  }

  if (!texture_)
  {
    texture_ = std::make_shared<sf::Texture>();
    if (!texture_->loadFromFile("gfx/ball.png"))
    {
      abort();
    }
    texture_->setSmooth(true);
  }
  sprite_.setTexture(texture_.get());
  sprite_.setRadius(radius_);
  sprite_.setFillColor(color);
}

Ball::Type Ball::type() const
{
  return type_;
}

std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>> Ball::split() const
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

  float x_pos = pos_.x + radius_ - type_properties_.at(type).radius;
  if (x_pos + type_properties_.at(type).radius * 2 > window_.getSize().x)
  {
    x_pos = window_.getSize().x - type_properties_.at(type).radius * 2;
  }

  return {
    std::make_shared<Ball>(window_, sf::Vector2f(x_pos, pos_.y),
                           type, color_, Ball::Direction::West),
    std::make_shared<Ball>(window_, sf::Vector2f(x_pos, pos_.y),
                             type, color_, Ball::Direction::East)
    };
}

sf::Color Ball::color() const
{
  return color_;
}

void Ball::move()
{
  if (pos_.x < 0 || pos_.x + sprite_.getGlobalBounds().width > window_.getSize().x)
  {
    x_velocity_ = -x_velocity_;
  }
  if (pos_.y < 0)
  {
    y_velocity_ = std::sin(angle_) * velocity_;
  }
  else if (pos_.y + radius_ * 2 > window_.getSize().y)
  {
    y_velocity_ = -(std::sin(angle_) * velocity_);
  }
  y_velocity_ += 0.214F;
  pos_.x += x_velocity_ / 10;
  pos_.y += y_velocity_ / 10;
}

void Ball::draw()
{
  sprite_.setPosition(pos_.x, pos_.y);
  window_.draw(sprite_);
}

bool Ball::hasPoint(int x, int y) const
{
  sf::FloatRect boundingBox = sprite_.getGlobalBounds();

  if (boundingBox.contains(sf::Vector2f(x, y)))
  {
    return true;
  }
  return false;
}

const std::map<Ball::Type, Ball::TypeProperties> Ball::type_properties_ = {
  { Ball::Type::Large, { 40.f, 40.f } },
  { Ball::Type::Medium, { 30.f, 35.f } },
  { Ball::Type::Small, { 20.f, 30.f } },
  { Ball::Type::Tiny, { 10.f, 20.f } }
};

std::shared_ptr<sf::Texture> Ball::texture_;
