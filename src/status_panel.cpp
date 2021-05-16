#include "status_panel.h"

namespace res = thor::Resources;

StatusPanel::StatusPanel(const std::string& level_name, const std::string& player_name, int player_lives, thor::ResourceHolder<sf::Texture, std::string>& resources)
  : level_name_(level_name)
  , player_name_(player_name)
  , player_lives_(player_lives)
  , player_texture_(resources.acquire("player", res::fromFile<sf::Texture>("data/player.png"), res::Reuse)) {
  if (!font_.loadFromFile("data/atarian_system_extended_bold.ttf")) {
    abort();
  }
}

void StatusPanel::draw(sf::RenderTarget& target, sf::RenderStates) const {
  drawLevelName(target);
  drawPlayerName(target);
  drawPlayerLives(target);
}

void StatusPanel::drawLevelName(sf::RenderTarget& target) const {
  sf::Text text(level_name_, font_);
  text.setCharacterSize(10);
  auto center = target.getView().getSize().x / 2 - text.getGlobalBounds().width / 2;
  text.setPosition(center, 207);
  target.draw(text);
}

void StatusPanel::drawPlayerName(sf::RenderTarget& target) const {
  sf::Text text(player_name_, font_);
  text.setPosition(16, 207);
  text.setCharacterSize(10);
  target.draw(text);
}

void StatusPanel::drawPlayerLives(sf::RenderTarget& target) const {
  for (int x=16; x < 16 * player_lives_; x += 16) {
    if (x == 16 * 5) {
      sf::Text text(std::to_string(player_lives_), font_);
      text.setPosition(static_cast<float>(x + 2), 221);
      text.setCharacterSize(16);
      target.draw(text);
      break;
    }
    sf::Sprite sprite;
    sprite.setTexture(player_texture_);
    sprite.setTextureRect(sf::IntRect(154, 44, 16, 16));
    sprite.setPosition(static_cast<float>(x), 223);
    target.draw(sprite);
  }
}
