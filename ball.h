#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

#include <memory>

class Ball
{
public:
  enum class Type
  {
    Large,
    Medium,
    Small,
    Tiny
  };
  enum class Direction : int
  {
    West,
    East
  };

  Ball(sf::RenderWindow& window, sf::Vector2f pos, Type type, sf::Color color,
       Direction direction);

  Type type() const;

  std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>> split() const;

  sf::Color color() const;

  void move();

  void draw();

  bool hasPoint(int x, int y) const;

private:
  struct TypeProperties
  {
    float radius;
    float velocity;
  };

  sf::RenderWindow& window_;
  sf::Vector2f pos_;
  float radius_;
  float velocity_;
  float angle_;
  float x_velocity_;
  float y_velocity_;
  Type type_;
  sf::Color color_;
  sf::CircleShape sprite_;
  static std::shared_ptr<sf::Texture> texture_;
  static const std::map<Type, TypeProperties> type_properties_;
};

#endif // BALL_H
