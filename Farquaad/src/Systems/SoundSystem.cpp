// Copyright 2016 Bablawn3d5

#include <Farquaad/Systems/SoundSystem.h>
#include <Farquaad/Components/Sound.hpp>

void SoundSystem::update(ex::EntityManager & em,
  ex::EventManager & events, ex::TimeDelta dt) {

  // Allocate new sounds
  em.each<Sound>(
    [this, &dt](ex::Entity entity, Sound &sound) {
    if ( sound.sound != nullptr ) {
      return;
    }

    auto s = std::make_shared<sf::Sound>(soundbuff_holder.get<sf::SoundBuffer>(sound.name));
    s->setPitch(sound.pitch);
    s->setVolume(sound.volume);
    s->pause();
    sound.sound = s;
  });

  // Play sounds
  em.each<Sound>(
    [&dt](ex::Entity entity, Sound &sound) {
      if ( sound.sound == nullptr ) {
        return;
      }

      if ( sound.start_delay >= 0.0f ) {
      if ( sound.play_count > 0 ) {
        if ( sound.sound->getStatus() != sf::Sound::Status::Playing ) {
          sound.sound->play();
          --sound.play_count;
        } else {
          entity.remove<Sound>();
        }
      }
    } else {
      sound.start_delay -= static_cast<float>(dt.count());
    }
  });
}
