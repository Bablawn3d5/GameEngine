// Copyright 2016 Bablawn3d5
#include <Farquaad/Core/LevelResourceLoader.h>
#include <Farquaad/Components/Renderable.hpp>
#include <Farquaad/Thor/ResourceLoader.h>
#include <regex>
#include "Aseprite/SFML/convert.h"
#include "AsepriteRgbaConvert/loader.h"
#include <iostream>
#include <atomic>

namespace {

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsTexture(thor::ResourceHolder<TextureWithOffsets, std::string>& texture_holder, const std::string& s) {
  try {
    const auto& loadingFunc = [&s]() {
      auto tex_obj = std::make_unique<TextureWithOffsets>();
      tex_obj->image = *thor::Resources::fromFile<sf::Image>(s).load();
      return tex_obj;
    };
    texture_holder.acquire(s,
                          thor::ResourceLoader<TextureWithOffsets>
                          (loadingFunc, s),
                          thor::Resources::Reuse);

  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsSpriteSheetTexture(thor::ResourceHolder<TextureWithOffsets, std::string>& texture_holder, const std::string& s) {
  try {
    // TODO(SMA): Move me to a resource loader.
    const auto& loadingFunc = [&s]() {
      auto tex_obj = std::make_unique<TextureWithOffsets>();
      auto assprite = aseprite::load_sprite_from_file(s.c_str());
      tex_obj->image = aseprite::convertToSFML(assprite);
      tex_obj->offsets = aseprite::calcFrameOffsets(assprite);

      // HACK(SMA) : Just load animation data while we're at it here.
      auto& offsets = tex_obj->offsets;
      AnimationMap& map = tex_obj->anims;
      for ( auto& tag : assprite.tags ) {
        Frame a;
        assert(tag.from <= tag.to);
        assert(tag.to < offsets.size());
        assert(tag.to < assprite.frames.size());
        assert(offsets.size() == assprite.frames.size());
        // Copy frame rects, assuming from -> to always specifies a range of frames.
        auto offsetStart = offsets.begin() + tag.from;
        auto offsetEnd = offsets.begin() + tag.to + 1;
        std::vector<sf::IntRect> anim_offsets(offsetStart, offsetEnd);
        // Copy frame delay data
        std::vector<entityx::TimeDelta> anim_delays;
        assert(anim_offsets.size() > 0);
        anim_delays.resize(anim_offsets.size());
        std::transform(assprite.frames.begin() + tag.from, assprite.frames.begin() + tag.to + 1,
                       anim_delays.begin(), [](const auto& elem) {
          // Convert MS to Sec
          return static_cast<entityx::TimeDelta>(elem.duration / 1000.0f);
        });
        a.loop_type = tag.loop_direction;
        a.frame_offsets = anim_offsets;
        a.frame_delay = anim_delays;

        // Assume there's not multiple tags with the same name.
        assert(map.find(tag.name) == map.end());
        map[tag.name] = a;
      }
      return tex_obj;
    };

    texture_holder.acquire(s,
                          thor::ResourceLoader<TextureWithOffsets>
                          (loadingFunc, s),
                          thor::Resources::Reuse);
  }

  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }

}

void LoadAsFontTexture(thor::ResourceHolder<sf::Font, std::string>& font_holder, const std::string& s) {
  try {
   font_holder.acquire(s,
                      thor::Resources::fromFile<sf::Font>(s),
                      thor::Resources::Reuse);
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}

} // Annon Namespace

// Queues a resoruce by name into the syncornized loading queue.
void LevelResoruceLoader::queue_load(const std::string& s) {
  const std::regex regex_ase (".(ase|aseprite)$");
  const std::regex regex_tex (".(png|bmp)$");
  const std::regex regex_font (".(ttf)$");
  const std::regex regex_sound (".(wav)$");
  const std::regex regex_json (".(json)$");
  if( std::regex_search(s, regex_json) ) {
    this->queue([this, s]{
      std::cout << "Loading file: " << s << std::endl;
      this->json_holder.acquire(s,
                      Resources::loadJSON(s),
                      thor::Resources::Reuse);
      return;
    });
  } else if( std::regex_search(s, regex_tex) ) {
    this->queue([this, s]{
      std::cout << "Loading file: " << s << std::endl;
      LoadAsTexture(this->texture_holder, s);
    });
  } else if( std::regex_search(s, regex_ase) ) {
    this->queue([this, s]{
      std::cout << "Loading file: " << s << std::endl;
      LoadAsSpriteSheetTexture(this->texture_holder, s);
    });
  } else if( std::regex_search(s, regex_font) ) {
    this->queue([this, s]{
      std::cout << "Loading file: " << s << std::endl;
      LoadAsFontTexture(this->font_holder, s);
    });
  } else if( std::regex_search(s, regex_sound) ) {
    this->queue([this, s]{
      std::cout << "Loading file: " << s << std::endl;
      this->soundbuff_holder.acquire(s,
                     thor::Resources::fromFile<sf::SoundBuffer>(s),
                     thor::Resources::Reuse);
    });
  } else {
    //Error
    assert(false && "Something terrible has happened.");
  }
}

void LevelResoruceLoader::queue(std::function<void()> callback) {
  std::lock_guard<std::mutex> lock(syncMutex);
  syncQueue.push(callback);
  loadCondition.notify_one();
}

void LevelResoruceLoader::joins() {
  std::atomic_flag isDone = ATOMIC_FLAG_INIT;
  // Aquire our own atomic, and let the loading thread clear it.
  isDone.test_and_set(std::memory_order_relaxed);
  this->queue([&isDone]{ isDone.clear(std::memory_order_release); });
  while ( isDone.test_and_set(std::memory_order_acquire) ) {
    std::this_thread::yield(); 
  }
  assert(syncQueue.empty());
  return;
}

void LevelResoruceLoader::loading() {
  // Do the sync requests
  std::unique_lock<std::mutex> lock(syncMutex);
  loadCondition.wait(lock, [this] {
    return !syncQueue.empty();
  });
  // Get next callback
  auto callback = syncQueue.front();
  syncQueue.pop();
  lock.unlock();

  // Call it
  callback();
}

// Returns a shared pointer to the resource.
template<typename T>
const T& LevelResoruceLoader::get(const std::string& s) const {
  static_assert(sizeof(T) == 0, "this function has to be implemented for desired type");
}

// Workaround constexpr if doesn't exist.
template<>
const Json::Value& LevelResoruceLoader::get(const std::string& s) const {
  try {
    return json_holder[s];
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}

template<>
const sf::Font& LevelResoruceLoader::get(const std::string& s) const {
  try {
    return font_holder[s];
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}

template<>
const TextureWithOffsets& LevelResoruceLoader::get(const std::string& s) const {
  try {
    return texture_holder[s];
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}

template<>
const sf::SoundBuffer& LevelResoruceLoader::get(const std::string& s) const {
  try { 
    return soundbuff_holder[s];
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << s
      << " " << e.what() << std::endl;
    throw;
  }
}
