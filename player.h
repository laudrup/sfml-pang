#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include <memory>

class Player
{
public:
  enum class Direction
  {
    Left,
    Right
  };

  Player(sf::RenderWindow& window, sf::Vector2f pos);
  void move(Direction direction);
  void stand();
  void shoot();
  void draw();

private:
  sf::RenderWindow& window_;
  sf::Vector2f pos_;
  Direction direction_;
  sf::RectangleShape sprite_;
  int count_;
  static std::shared_ptr<sf::Texture> texture_;
  const std::vector<sf::IntRect> walk_textures_ = { sf::IntRect(10, 2, 32, 32),
                                                    sf::IntRect(44, 2, 32, 32),
                                                    sf::IntRect(78, 2, 32, 32),
                                                    sf::IntRect(112, 2, 32, 32),
                                                    sf::IntRect(146, 2, 32, 32) };
  std::vector<sf::IntRect>::const_iterator current_walk_texture_;
};

#endif // PLAYER_H
