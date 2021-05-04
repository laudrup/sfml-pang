#ifndef SHOT_H
#define SHOT_H

#include <SFML/Graphics.hpp>

#include <Thor/Animations.hpp>
#include <Thor/Resources.hpp>

#include <memory>

class Shot : public sf::RectangleShape {
public:
  Shot(sf::Vector2f pos,
       const sf::FloatRect& area,
       float initial_height,
       thor::ResourceHolder<sf::Texture, std::string>& resources);
  void update(sf::Time delta_time);
  sf::Vector2f position() const;
  sf::FloatRect bounds() const;

private:
  const std::vector<sf::IntRect> textures_ = {sf::IntRect(91, 0, 10, 200), sf::IntRect(19, 0, 10, 200)};
  const sf::FloatRect area_;
  const sf::Vector2f speed_{0.f, 150.f};
  thor::FrameAnimation shot_anim_;
  thor::Animator<sf::RectangleShape, std::string> anim_;
};

#endif // SHOT_H
