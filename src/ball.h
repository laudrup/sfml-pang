#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

class Ball : public sf::CircleShape {
public:
  enum class Type {
    Large,
    Medium,
    Small,
    Tiny,
  };
  enum class Direction : int {
    West = -1,
    East = 1,
  };
  Ball(Type type,
       sf::Color color,
       sf::Vector2f position,
       Direction direction,
       const sf::FloatRect& area,
       thor::ResourceHolder<sf::Texture, std::string>* resources);
  void update(const sf::Time delta_time);
  sf::FloatRect bounds() const;
  std::pair<Ball, Ball> split() const;
  Type type() const {
    return type_;
  }
  Direction direction() const {
    return direction_;
  }

private:
  Type type_;
  Direction direction_;
  float mass_;
  sf::Vector2f velocity_;
  sf::FloatRect area_;
  sf::Texture* texture_;
  thor::ResourceHolder<sf::Texture, std::string>* resources_;
};

#endif // BALL_H
