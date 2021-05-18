#pragma once

#include "ball.h"
#include "background.h"
#include "status_panel.h"
#include "player.h"
#include "sound_manager.h"

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include <string>
#include <vector>

class Game : public sf::Drawable {
public:
  Game(thor::ResourceHolder<sf::Texture, std::string>& resources, SoundManager& sound_manager);
  void handleEvent(sf::Event event);
  void update(sf::Time delta_time);
  void pause() { paused_ = !paused_; };

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  sf::FloatRect area_{8, 8, 376, 200};
  thor::ResourceHolder<sf::Texture, std::string>& resources_;
  SoundManager& sound_manager_;

  Background background_;
  StatusPanel status_panel_;
  Player player_;
  std::vector<Ball> balls_ = {
    Ball(Ball::Type::Large, sf::Color::Red, {384.f / 3, 208.f / 2}, Ball::Direction::East, area_, &resources_),
    Ball(Ball::Type::Medium, sf::Color::Yellow, {384.f / 2, 208.f / 2}, Ball::Direction::East, area_, &resources_),
    Ball(Ball::Type::Small, sf::Color::Green, {384.f / 3, 208.f / 2}, Ball::Direction::East, area_, &resources_),
    Ball(Ball::Type::Tiny, sf::Color::Blue, {384 / 1.5, 208.f / 3}, Ball::Direction::East, area_, &resources_)
  };
  sf::Clock clock_;
  sf::Time elapsed_time_;
  bool paused_ = false;
};
