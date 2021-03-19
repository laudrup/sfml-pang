#include "ball.h"
#include "player.h"
#include "shot.h"
#include "background.h"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>

#include <cassert>

int main()
{
  thor::ResourceHolder<sf::Texture, std::string> resources;
  sf::RenderWindow window(sf::VideoMode(384 * 2, 208 * 2), "SFML works!");
  sf::View view(sf::FloatRect(0, 0, 384, 208));
  window.setView(view);
  auto area = sf::IntRect(8, 8, background.size().x - 16, background.size().y - 16);
  Background background(window.getSize());
  std::vector<Ball> balls = {
    Ball(Ball::Type::Large, sf::Color::Red, {384 / 3, 208 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Medium, sf::Color::Yellow, {384 / 2, 208 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Small, sf::Color::Green, {384 / 3, 208 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Tiny, sf::Color::Blue, {384 / 1.5, 208 / 3}, Ball::Direction::West,
         area, &resources)
         };
  Player player(sf::Vector2f(area.width / 2, area.height - 8), area, resources);
  sf::Clock clock;
  sf::Clock AITimer;
  const sf::Time ai_time = sf::seconds(1.f) / 20.f;
  const sf::Vector2f gravity(0.f, 5.f);
  sf::Time elapsed_time = clock.restart();
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
      case sf::Event::KeyPressed:
      case sf::Event::KeyReleased:
        if(player.handleEvent(event))
        {
          break;
        }
      default:
        break;
      }
    }

    elapsed_time += clock.restart();
    while (elapsed_time >= ai_time)
    {
      for (auto &ball : balls)
      {
        ball.update(gravity, ai_time);
      }
      player.update(ai_time);
      elapsed_time -= ai_time;
    }
    for (const auto& shot : player.shots())
    {
      std::vector<Ball>::iterator ball_it = balls.begin();
      while (ball_it != balls.end())
      {
        if (ball_it->bounds().intersects(shot->bounds()))
        {
          if (ball_it->type() != Ball::Type::Tiny)
          {
            auto new_balls = ball_it->split();
            balls.erase(ball_it);
            balls.push_back(new_balls.first);
            balls.push_back(new_balls.second);
          }
          else
          {
            balls.erase(ball_it);
          }
          // TODO: Fix this. Will break if more than one shot
          player.removeShot();
          break;
        }
        else
        {
          ball_it++;
        }
      }
    }
    for (const auto& ball : balls)
    {
      if (ball.bounds().intersects(player.bounds()))
      {
        std::cerr << "You died" << std::endl;
        break;
      }
    }
    window.clear();
    window.draw(background);
    for (auto &ball : balls)
    {
      window.draw(ball);
    }
    for (auto& shot : player.shots())
    {
      window.draw(*shot);
    }
    window.draw(player);
    window.display();
  }
}
