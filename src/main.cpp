#include "background.h"
#include "ball.h"
#include "player.h"
#include "shot.h"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

namespace {

void save_screenshot(const sf::Texture& tex) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream os;
  os << std::put_time(&tm, "screenshot_%Y%m%d_%H%M%S.png");
  const auto fname = os.str();
  sf::Image img = tex.copyToImage();
  img.saveToFile(fname);
  std::cerr << "Screenshot saved to '" << fname << "'\n";
}

} // namespace

int main() {
  thor::ResourceHolder<sf::Texture, std::string> resources;
  sf::RenderWindow window(sf::VideoMode(384 * 2, 208 * 2), "SFML works!");
  sf::View view(sf::FloatRect(0, 0, 384, 208));
  window.setView(view);
  Background background(window.getSize());
  auto area = sf::IntRect(0, 0, background.size().x, background.size().y);
  std::vector<Ball> balls = {
      Ball(Ball::Type::Large, sf::Color::Red, {384 / 3, 208 / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Medium, sf::Color::Yellow, {384 / 2, 208 / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Small, sf::Color::Green, {384 / 3, 208 / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Tiny, sf::Color::Blue, {384 / 1.5, 208 / 3}, Ball::Direction::East, area, &resources)};
  Player player(sf::Vector2f(area.width / 2, area.height), area, resources);
  sf::Clock clock;
  const sf::Time ai_time = sf::seconds(1.f) / 20.f;
  sf::Time elapsed_time = clock.restart();
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::F12) {
          sf::Texture tex;
          tex.create(window.getSize().x, window.getSize().y);
          tex.update(window);
          save_screenshot(tex);
          break;
        }
      case sf::Event::KeyReleased:
        if (player.handleEvent(event)) {
          break;
        }
      default:
        break;
      }
    }

    if (player.state() == Player::State::Dead) {
      std::cout << "You died!\n";
      exit(0);
    }

    elapsed_time += clock.restart();
    while (elapsed_time >= ai_time) {
      if (player.state() == Player::State::Alive) {
        for (auto& ball : balls) {
          ball.update(ai_time);
        }
      }
      player.update(ai_time);
      elapsed_time -= ai_time;
    }
    if (player.state() == Player::State::Alive) {
      for (const auto& shot : player.shots()) {
        std::vector<Ball>::iterator ball_it = balls.begin();
        while (ball_it != balls.end()) {
          if (ball_it->bounds().intersects(shot->bounds())) {
            if (ball_it->type() != Ball::Type::Tiny) {
              auto new_balls = ball_it->split();
              balls.erase(ball_it);
              balls.push_back(new_balls.first);
              balls.push_back(new_balls.second);
            } else {
              balls.erase(ball_it);
            }
            // TODO: Fix this. Will break if more than one shot
            player.removeShot();
            break;
          } else {
            ball_it++;
          }
        }
      }
      for (const auto& ball : balls) {
        if (ball.bounds().intersects(player.bounds())) {
          // TODO: Do pixel perfect collision detection. This will
          // kill the player even if the ball hasn't actually touched
          player.die(ball.direction());
          break;
        }
      }
    }
    window.clear();
    window.draw(background);
    for (auto& ball : balls) {
      window.draw(ball);
    }
    for (auto& shot : player.shots()) {
      window.draw(*shot);
    }
    window.draw(player);
    window.display();
    if (balls.empty()) {
      std::cout << "Level complete!\n";
      exit(0);
    }
  }
}
