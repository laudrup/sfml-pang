#pragma once

#include <SFML/Audio.hpp>

#include <map>
#include <vector>
#include <string>

class SoundManager {
public:
  SoundManager();
  void play(const std::string& snd);

private:
  std::map<std::string, sf::SoundBuffer> buffers_;
  std::vector<sf::Sound> sounds_;
};

