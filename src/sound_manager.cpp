#include "sound_manager.h"

SoundManager::SoundManager() {
  sf::SoundBuffer buffer;

  buffer.loadFromFile("data/pop.ogg");
  buffers_["pop"] = buffer;
  buffer.loadFromFile("data/death.ogg");
  buffers_["death"] = buffer;
}

void SoundManager::play(const std::string& snd) {
  sounds_.erase(std::remove_if(sounds_.begin(),
                               sounds_.end(),
                               [](const auto& sound) {
                                 return sound.getStatus() == sf::SoundSource::Status::Stopped;
                               }),
                sounds_.end());
  sounds_.emplace_back(buffers_.at(snd)).play();
}
