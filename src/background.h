#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

class Background : public sf::Drawable
{
public:
  Background(const sf::Vector2u& area);
  sf::Vector2f scale() const { return sprite_.getScale(); }
  sf::Vector2f size() const {
    return {sprite_.getGlobalBounds().width, sprite_.getGlobalBounds().height}; }

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  std::vector<sf::Texture> textures_;
  sf::Sprite sprite_;
};

#endif // BACKGROUND_H
