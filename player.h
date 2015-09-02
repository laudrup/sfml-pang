#ifndef PLAYER_H
#define PLAYER_H

#include "shot.h"

#include <SFML/Graphics.hpp>

#include <Thor/Animations.hpp>
#include <Thor/Resources.hpp>

#include <vector>
#include <string>

class Player : public sf::Drawable, public sf::Transformable
{
public:
  enum class Direction
  {
    Left,
    Right,
    Stopped
  };

  Player(sf::Vector2f pos, const sf::IntRect& area,
         thor::ResourceHolder<sf::Texture, std::string>& resources);
  bool handleEvent(sf::Event event);
  void update(sf::Time delta_time);
  std::vector<std::shared_ptr<Shot>> shots() const { return shots_; }
  sf::FloatRect bounds() const;
  void removeShot();

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  Direction direction_;
  sf::Sprite sprite_;
  const sf::IntRect area_;
  const std::vector<sf::IntRect> walk_textures_ = { sf::IntRect(10, 2, 32, 32),
                                                    sf::IntRect(44, 2, 32, 32),
                                                    sf::IntRect(78, 2, 32, 32),
                                                    sf::IntRect(112, 2, 32, 32),
                                                    sf::IntRect(146, 2, 32, 32) };
  const std::vector<sf::IntRect> shoot_textures_ = { sf::IntRect(10, 112, 32, 32),
                                                     sf::IntRect(44, 112, 32, 32),
                                                     sf::IntRect(10, 112, 32, 32) };
  thor::FrameAnimation walk_anim_;
  thor::FrameAnimation shoot_anim_;
  thor::Animator<sf::Sprite, std::string> anim_;
  const sf::Vector2f speed_ {200.f, 200.f};
  std::vector<std::shared_ptr<Shot>> shots_;
  thor::ResourceHolder<sf::Texture, std::string>& resources_;
};

#endif // PLAYER_H
