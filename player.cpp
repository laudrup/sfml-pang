#include "player.h"

Player::Player(sf::RenderWindow& window, sf::Vector2f pos)
  : window_(window),
    pos_(pos),
    direction_(Direction::Right),
    sprite_(sf::Vector2f(32, 32)),
    count_(0),
    current_walk_texture_(walk_textures_.begin())
{
  if (!texture_)
  {
    texture_ = std::make_shared<sf::Texture>();
    if (!texture_->loadFromFile("gfx/player.png"))
    {
      abort();
    }
    texture_->setSmooth(true);
  }
  sprite_.setTexture(texture_.get());
  sprite_.setScale(2.f, 2.f);
  sprite_.setTextureRect(*current_walk_texture_);
}

void Player::move(Direction direction)
{
  if (direction == Direction::Left) // && pos_.x > sprite_.getGlobalBounds().width)
  {
    if (direction_ == Direction::Right)
    {
      sprite_.setScale(-2.f, 2.f);
      pos_.x += 64;
      direction_ = Direction::Left;
    }
    else if (pos_.x > sprite_.getGlobalBounds().width)
    {
      pos_.x--;
    }
  }
  else if (direction == Direction::Right)
  {
    if (direction_ == Direction::Left)
    {
      sprite_.setScale(2.f, 2.f);
      pos_.x -= 64;
      direction_ = Direction::Right;
    }
    else if (pos_.x < window_.getSize().x - sprite_.getGlobalBounds().width)
    {
      pos_.x++;
    }
  }
  if (count_++ % 10 == 0)
  {
    if (++current_walk_texture_ == walk_textures_.end())
    {
      current_walk_texture_ = walk_textures_.begin();
    }
    sprite_.setTextureRect(*current_walk_texture_);
  }
}

void Player::stand()
{
  current_walk_texture_ = walk_textures_.begin();
  sprite_.setTextureRect(*current_walk_texture_);
}

void Player::shoot()
{
  sprite_.setTextureRect(sf::IntRect(10, 112, 32, 32));
}

void Player::draw()
{
  sprite_.setPosition(pos_.x, pos_.y);
  window_.draw(sprite_);
}

std::shared_ptr<sf::Texture> Player::texture_;
