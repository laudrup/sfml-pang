#include <SFML/Graphics.hpp>

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>

#include <cassert>

class Ball
{
public:
  enum class Type
  {
    Large,
    Medium,
    Small,
    Tiny
  };
  enum class Direction : int
  {
    West,
    East
  };

  Ball(sf::RenderWindow& window, sf::Vector2f pos, Type type, sf::Color color,
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

  Type type() const
  {
    return type_;
  }

  std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>> split() const
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

  sf::Color color() const
  {
    return color_;
  }

  void move()
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

  void draw()
  {
    sprite_.setPosition(pos_.x, pos_.y);
    window_.draw(sprite_);
  }

  bool hasPoint(int x, int y) const
  {
    sf::FloatRect boundingBox = sprite_.getGlobalBounds();

    if (boundingBox.contains(sf::Vector2f(x, y)))
    {
      return true;
    }
    return false;
  }

private:
  struct TypeProperties
  {
    float radius;
    float velocity;
  };

  sf::RenderWindow& window_;
  sf::Vector2f pos_;
  float radius_;
  float velocity_;
  float angle_;
  float x_velocity_;
  float y_velocity_;
  Type type_;
  sf::Color color_;
  sf::CircleShape sprite_;
  static std::shared_ptr<sf::Texture> texture_;
  static const std::map<Type, TypeProperties> type_properties_;
};

const std::map<Ball::Type, Ball::TypeProperties> Ball::type_properties_ = {
  { Ball::Type::Large, { 40.f, 40.f } },
  { Ball::Type::Medium, { 30.f, 35.f } },
  { Ball::Type::Small, { 20.f, 30.f } },
  { Ball::Type::Tiny, { 10.f, 20.f } }
};

std::shared_ptr<sf::Texture> Ball::texture_;

int main()
{
  sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML works!");
  std::vector<std::shared_ptr<Ball>> balls = { std::make_shared<Ball>(window, sf::Vector2f(200, 200), Ball::Type::Large, sf::Color::Red, Ball::Direction::East),
                                               std::make_shared<Ball>(window, sf::Vector2f(200, 200), Ball::Type::Medium, sf::Color::Blue, Ball::Direction::West),
                                               std::make_shared<Ball>(window, sf::Vector2f(200, 200), Ball::Type::Small, sf::Color::Green, Ball::Direction::East),
                                               std::make_shared<Ball>(window, sf::Vector2f(200, 200), Ball::Type::Tiny, sf::Color::Yellow, Ball::Direction::West) };
  sf::Clock clock;
  sf::Clock AITimer;
  const sf::Time AITime = sf::seconds(0.01f);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch(event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          for (auto it = balls.begin(); it != balls.end(); ++it)
          {
            auto ball = *it;
            if (ball->hasPoint(event.mouseButton.x, event.mouseButton.y))
            {
              balls.erase(it);
              if (ball->type() != Ball::Type::Tiny)
              {
                auto new_balls = ball->split();
                balls.push_back(new_balls.first);
                balls.push_back(new_balls.second);
              }
              break;
            }
          }
        }
        break;
      default:
        break;
      }
    }
    if (AITimer.getElapsedTime() > AITime)
    {
      AITimer.restart();
      window.clear();
      for (auto& ball : balls)
      {
        ball->move();
        ball->draw();
      }
      window.display();
    }
  }
  return 0;
}
