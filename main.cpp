#include <SFML/Graphics.hpp>

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>

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
  Ball(sf::RenderWindow& window, float x, float y, Type type, sf::Color color)
    : window_(window),
      x_(x),
      y_(y),
      angle_(M_PI * 2 / 5),
      type_(type)
  {
    switch (type_)
    {
    case Type::Large:
      {
        radius_ = 40.f;
        velocity_ = 40.f;
      }
      break;
    case Type::Medium:
      {
        radius_ = 30.f;
        velocity_ = 35.f;
      }
      break;
    case Type::Small:
      {
        radius_ = 20.f;
        velocity_ = 30.f;
      }
      break;
    case Type::Tiny:
      {
        radius_ = 10.f;
        velocity_ = 20.f;
      }
      break;
    }

    x_velocity_ = std::cos(angle_) * velocity_;
    y_velocity_ = std::sin(angle_) * velocity_;
    sprite_.setRadius(radius_);
    sprite_.setFillColor(color);

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
  }

  Type type() const
  {
    return type_;
  }

  void move()
  {
    if (x_ < 0 || x_ + radius_ * 2 > window_.getSize().x)
    {
      x_velocity_ = -x_velocity_;
    }
    if (y_ < 0)
    {
      y_velocity_ = (std::sin(angle_) * velocity_);
    }
    else if(y_ + radius_ * 2 > window_.getSize().y)
    {
      y_velocity_ = -(std::sin(angle_) * velocity_);
    }
    else
    {
      y_velocity_ += 0.214F;
    }
    x_ += x_velocity_ / 10;
    y_ += y_velocity_ / 10;
  }

  void draw()
  {
    sprite_.setPosition(x_, y_);
    window_.draw(sprite_);
  }

private:
  sf::RenderWindow& window_;
  float x_;
  float y_;
  float radius_;
  float velocity_;
  float angle_;
  float x_velocity_;
  float y_velocity_;
  Type type_;
  sf::CircleShape sprite_;
  static std::shared_ptr<sf::Texture> texture_;
};

std::shared_ptr<sf::Texture> Ball::texture_;

int main()
{
  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
  std::vector<Ball> balls = { Ball(window, 10, 10, Ball::Type::Large, sf::Color::Red),
                              Ball(window, 10, 10, Ball::Type::Medium, sf::Color::Blue),
                              Ball(window, 10, 10, Ball::Type::Small, sf::Color::Green),
                              Ball(window, 10, 10, Ball::Type::Tiny, sf::Color::Yellow) };
  sf::Clock clock;
  sf::Clock AITimer;
  const sf::Time AITime = sf::seconds(0.01f);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    if (AITimer.getElapsedTime() > AITime)
    {
      AITimer.restart();
      window.clear();
      for (auto& ball : balls)
      {
        ball.move();
        ball.draw();
      }
      window.display();
    }
  }
  return 0;
}
