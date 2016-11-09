// Copyright 2016 Bablawn3d5
#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <json/json.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Resources.hpp>
// Forward declare here
struct TextureWithOffsets;

class LevelResoruceLoader {
public:
  thor::ResourceHolder<Json::Value, std::string> json_holder;
  thor::ResourceHolder<sf::Font, std::string> font_holder;
  thor::ResourceHolder<TextureWithOffsets, std::string> texture_holder;
  thor::ResourceHolder<sf::SoundBuffer, std::string> soundbuff_holder;

  // Queue: adds a job to the loading queue.
  void queue(std::function<void()> callback);

  // Loading: to be called by loading thread.
  void loading();

  // Blocks calling thread till queue is empty (no tasks left);
  void joins();

  // queue_load asynchronously loads a resource
  void queue_load(const std::string& s);

  // Returns a resource if its loaded.
  template<typename T>
  const T& get(const std::string& s) const;
private:
  std::queue<std::function<void()>> syncQueue;
  std::mutex syncMutex;
  std::condition_variable loadCondition;
  std::atomic_flag loading_lock = ATOMIC_FLAG_INIT;
};
