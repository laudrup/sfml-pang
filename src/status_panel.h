#ifndef STATUS_PANEL_H
#define STATUS_PANEL_H

#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include <string>

class StatusPanel : public sf::Drawable {
public:
  StatusPanel(const std::string& level_name, const std::string& player_name, int player_lives, thor::ResourceHolder<sf::Texture, std::string>& resources);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void drawLevelName(sf::RenderTarget& target) const;
  void drawPlayerName(sf::RenderTarget& target) const;
  void drawPlayerLives(sf::RenderTarget& target) const;

  std::string level_name_;
  std::string player_name_;
  int player_lives_;
  sf::Font font_;
  sf::Texture player_texture_;
};

#endif // STATUS_PANEL_H
