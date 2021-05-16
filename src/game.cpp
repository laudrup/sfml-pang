#include "game.h"

#include <cstdlib>
#include <iostream>

Game::Game(thor::ResourceHolder<sf::Texture, std::string>& resources, SoundManager& sound_manager)
  : resources_(resources)
  , sound_manager_(sound_manager)
  , status_panel_("Mount Doom", "Freja", 7, resources)
  , player_(sf::Vector2f(376.f / 2, 200), sf::FloatRect(8, 8, 376, 200), resources)
  , elapsed_time_(clock_.restart()) {
}

void Game::handleEvent(sf::Event event) {
  player_.handleEvent(event);
}

void Game::update(sf::Time ai_time) {
  elapsed_time_ += clock_.restart();

  if (player_.state() == Player::State::Dead) {
    std::cout << "You died!\n";
    exit(0);
  }

  while (elapsed_time_ >= ai_time) {
    if (player_.state() == Player::State::Alive) {
      for (auto& ball : balls_) {
        ball.update(ai_time);
      }
    }
    player_.update(ai_time);
    elapsed_time_ -= ai_time;
    if (player_.state() == Player::State::Alive) {
      for (const auto& shot : player_.shots()) {
        std::vector<Ball>::iterator ball_it = balls_.begin();
        while (ball_it != balls_.end()) {
          if (ball_it->getGlobalBounds().intersects(shot->getGlobalBounds())) {
            sound_manager_.play("pop");
            if (ball_it->type() != Ball::Type::Tiny) {
              auto new_balls = ball_it->split();
              balls_.erase(ball_it);
              balls_.push_back(new_balls.first);
              balls_.push_back(new_balls.second);
            } else {
              balls_.erase(ball_it);
            }
            // TODO: Fix this. Will break if more than one shot
            player_.removeShot();
            break;
          } else {
            ball_it++;
          }
        }
      }
      for (const auto& ball : balls_) {
        if (ball.getGlobalBounds().intersects(player_.getGlobalBounds())) {
          // TODO: Do pixel perfect collision detection. This will
          // kill the player even if the ball hasn't actually touched
          player_.die(ball.direction());
          sound_manager_.play("death");
          break;
        }
      }
    }
  }
  if (balls_.empty()) {
    std::cout << "Level complete!\n";
    exit(0);
  }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates) const {
  target.draw(background_);
  target.draw(player_);
  target.draw(status_panel_);

  for (auto& ball : balls_) {
    target.draw(ball);
  }

  for (auto& shot : player_.shots()) {
    target.draw(*shot);
  }
}
