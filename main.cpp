#include "ball.h"
#include "player.h"
#include "shot.h"
#include "background.h"

#include <SFML/Graphics.hpp>

#include "collision.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>

#include <cassert>

int main()
{
  thor::ResourceHolder<sf::Texture, std::string> resources;
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
  window.setFramerateLimit(60);
  Background background(window.getSize(), resources);
  auto area = sf::IntRect(8 * background.scale().x,
                          8 * background.scale().y,
                          window.getSize().x - 16 * background.scale().x,
                          window.getSize().y - 16 * background.scale().y);
  std::vector<Ball> balls = {
    Ball(Ball::Type::Large, sf::Color::Red, {800 / 3, 600 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Medium, sf::Color::Yellow, {800 / 2, 600 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Small, sf::Color::Green, {800 / 3, 600 / 2}, Ball::Direction::East,
         area, &resources),
    Ball(Ball::Type::Tiny, sf::Color::Blue, {800 / 1.5, 600 / 3}, Ball::Direction::West,
         area, &resources)
         };
  Player player(sf::Vector2f(area.width / 2, area.height - 8 * background.scale().y),
                area, resources);
  sf::Clock clock;
  sf::Clock AITimer;
  const sf::Time ai_time = sf::seconds(1.f) / 20.f;
  const sf::Vector2f gravity(0.f, 30.f);
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
