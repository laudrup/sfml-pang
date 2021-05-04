#ifndef PLAYER_H
#define PLAYER_H

#include "ball.h"
#include "shot.h"

#include <SFML/Graphics.hpp>

#include <Thor/Animations.hpp>
#include <Thor/Resources.hpp>

#include <array>
#include <string>

class Player : public sf::Drawable, public sf::Transformable {
public:
  enum class Direction {
    Left,
    Right,
    Stopped,
  };

  enum class State {
    Alive,
    Dying,
    Dead,
  };

  Player(sf::Vector2f pos, const sf::FloatRect& area, thor::ResourceHolder<sf::Texture, std::string>& resources);
  bool handleEvent(sf::Event event);
  void update(sf::Time delta_time);
  std::vector<std::shared_ptr<Shot>> shots() const {
    return shots_;
  }
  sf::FloatRect bounds() const;
  void removeShot();
  void die(Ball::Direction direction);
  State state() const {
    return state_;
  }

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  Direction direction_;
  State state_;
  sf::Sprite sprite_;
  const sf::FloatRect area_;
  const std::array<sf::IntRect, 5> walk_textures_ = {
      sf::IntRect(10, 2, 32, 32),
      sf::IntRect(44, 2, 32, 32),
      sf::IntRect(78, 2, 32, 32),
      sf::IntRect(112, 2, 32, 32),
      sf::IntRect(146, 2, 32, 32),
  };
  const std::array<sf::IntRect, 3> shoot_textures_ = {
      sf::IntRect(10, 112, 32, 32),
      sf::IntRect(44, 112, 32, 32),
      sf::IntRect(10, 112, 32, 32),
  };
  const std::array<sf::IntRect, 2> death_textures_ = {
      sf::IntRect(78, 112, 48, 32),
      sf::IntRect(128, 112, 48, 32),
  };
  thor::FrameAnimation walk_anim_;
  thor::FrameAnimation shoot_anim_;
  thor::Animator<sf::Sprite, std::string> anim_;
  const sf::Vector2f speed_{120.f, 120.f};
  std::vector<std::shared_ptr<Shot>> shots_;
  thor::ResourceHolder<sf::Texture, std::string>& resources_;
  sf::Vector2f death_anim_velocity_;
};

#endif // PLAYER_H
