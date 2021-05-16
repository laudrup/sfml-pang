#include "game.h"
#include "sound_manager.h"

#include <SFML/Graphics.hpp>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void save_screenshot(const sf::RenderWindow& window) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream os;
  os << std::put_time(&tm, "screenshot_%Y%m%d_%H%M%S.png");
  const auto fname = os.str();

  sf::Texture tex;
  tex.create(window.getSize().x, window.getSize().y);
  tex.update(window);

  sf::Image img = tex.copyToImage();
  img.saveToFile(fname);
  std::cerr << "Screenshot saved to '" << fname << "'\n";
}

} // namespace

int main() {
  const auto ai_time = sf::seconds(1.f) / 20.f;

  sf::RenderWindow window(sf::VideoMode(384 * 2, 240 * 2), "SFML works!");
  sf::View view(sf::FloatRect(0, 0, 384, 240));
  window.setView(view);

  thor::ResourceHolder<sf::Texture, std::string> resources;
  SoundManager sound_manager;
  Game game(resources, sound_manager);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::F12) {
          save_screenshot(window);
          break;
        }
        [[fallthrough]];
      case sf::Event::KeyReleased:
        game.handleEvent(event);
        break;
      default:
        break;
      }
    }

    game.update(ai_time);

    window.clear();
    window.draw(game);
    window.display();
  }
}
