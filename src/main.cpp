#include "background.h"
#include "ball.h"
#include "player.h"
#include "shot.h"
#include "status_panel.h"
#include "sound_manager.h"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

void save_screenshot(const sf::RenderWindow& window) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream os;
  os << std::put_time(&tm, "screenshot_%Y%m%d_%H%M%S.png");
  const auto fname = os.str();

  sf::Texture tex;
  tex.create(window.getSize().x, window.getSize().y);
  tex.update(window);

  sf::Image img = tex.copyToImage();
  img.saveToFile(fname);
  std::cerr << "Screenshot saved to '" << fname << "'\n";
}

} // namespace

int main() {
  thor::ResourceHolder<sf::Texture, std::string> resources;
  sf::RenderWindow window(sf::VideoMode(384 * 2, 240 * 2), "SFML works!");
  sf::View view(sf::FloatRect(0, 0, 384, 240));
  window.setView(view);
  Background background;
  StatusPanel status_panel("Mount Doom", "Freja", 7, resources);
  SoundManager sound_manager;

  auto area = sf::FloatRect(8, 8, background.size().x - 8, background.size().y - 8);
  std::vector<Ball> balls = {
      Ball(Ball::Type::Large, sf::Color::Red, {384.f / 3, 208.f / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Medium, sf::Color::Yellow, {384.f / 2, 208.f / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Small, sf::Color::Green, {384.f / 3, 208.f / 2}, Ball::Direction::East, area, &resources),
      Ball(Ball::Type::Tiny, sf::Color::Blue, {384 / 1.5, 208.f / 3}, Ball::Direction::East, area, &resources)};
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
          save_screenshot(window);
          break;
        }
        [[fallthrough]];
      case sf::Event::KeyReleased:
        player.handleEvent(event);
        break;
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
          if (ball_it->getGlobalBounds().intersects(shot->getGlobalBounds())) {
            sound_manager.play("pop");
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
        if (ball.getGlobalBounds().intersects(player.getGlobalBounds())) {
          // TODO: Do pixel perfect collision detection. This will
          // kill the player even if the ball hasn't actually touched
          player.die(ball.direction());
          sound_manager.play("death");
          break;
        }
      }
    }
    window.clear();
    window.draw(background);
    window.draw(status_panel);
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
