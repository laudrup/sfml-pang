#include "ball.h"

#include <SFML/Graphics.hpp>

#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>

#include <cassert>

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
