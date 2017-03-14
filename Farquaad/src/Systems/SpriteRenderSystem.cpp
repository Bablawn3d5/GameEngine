// Copyright 2016 Bablawn3d5
#include <Farquaad/Systems/SpriteRenderSystem.h>
#include <Farquaad/Components.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <cctype>
#include <algorithm>
#include <utility>
#include  <functional>

#include "Aseprite/SFML/convert.h"
#include "AsepriteRgbaConvert/loader.h"

namespace {
// String comparision helper, returns true if two strings are equal regardless of case.
bool case_insensitive_string_eq(const std::string& lhs, const std::string& rhs) {
  return (lhs.size() == rhs.size()) &&
         std::equal(lhs.begin(),
                    lhs.end(),
                    rhs.begin(),
                    [](const auto& lhs, const auto& rhs) {
                      return std::toupper(lhs) == std::toupper(rhs);
                    });
}

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsTexture(const LevelResoruceLoader& holder, 
                   thor::ResourceHolder<sf::Texture, std::string>& tex, 
                   Renderable &renderable) {
    auto& image = holder.get<TextureWithOffsets>(renderable.texture_name);
    auto& texture = tex.acquire(renderable.texture_name,
                                          thor::Resources::fromImage<sf::Texture>(image.image),
                                          thor::Resources::Reuse);
    auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(texture);
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(sprite);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(sprite).get();
}

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsSpriteSheetTexture(const LevelResoruceLoader& holder, 
                              thor::ResourceHolder<sf::Texture, std::string>& tex, 
                              Renderable &renderable) {
    auto& image = holder.get<TextureWithOffsets>(renderable.texture_name);
    auto& texture = tex.acquire(renderable.texture_name,
                                          thor::Resources::fromImage<sf::Texture>(image.image),
                                          thor::Resources::Reuse);
    auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(texture);
    renderable.animations = image.anims;
    // HACK (SMA) : Set current animation to be the first frame
    // if its unset
    if ( renderable.current_animation.length() == 0 && renderable.animations.size() != 0) {
      renderable.current_animation = renderable.animations.begin()->first;
    }
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(sprite);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(sprite).get();
    renderable.sprite = std::static_pointer_cast<sf::Sprite>(sprite).get();

} // Anon Namespace

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsFontTexture(const LevelResoruceLoader& holder, Renderable &renderable) {
    auto& font = holder.get<sf::Font>(renderable.font_name);
    auto text = std::make_shared<sf::Text>(renderable.font_string, font, renderable.font_size);
    text->setFillColor(sf::Color(renderable.r, renderable.g, renderable.b, renderable.a));
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(text);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(text).get();
}

} // anon namespace

void SpriteRenderSystem::update(ex::EntityManager & em,
                          ex::EventManager & events, ex::TimeDelta dt) {
    const std::regex regex (".(ase|aseprite)$");
    // Update drawables.
    em.each<Body, Renderable>(
        [this, &regex](ex::Entity entity, Body &body, Renderable &renderable) {
      // If dirty, then rebuild drawable
      if ( renderable.isDirty == true ) {
        renderable.drawable = nullptr;
      }
      // Resource exists, just exit it.
      else if ( renderable.drawable != nullptr ) {
        return;
      }

      // Create sprite from texture
      // TODO(SMA): Just combine renderable to have one name type, and have it loaded in
      // based on regex extendsions.
      if ( renderable.texture_name.length() != 0 ) {
        // If we're loading a .ase or .aseprite, treate as spritesheet
        if ( std::regex_search(renderable.texture_name, regex) ) {
          LoadAsSpriteSheetTexture(this->holder, this->texture_holder, renderable);
        } else {
          LoadAsTexture(this->holder, this->texture_holder, renderable);
        }
      } else if ( renderable.font_name.length() != 0 ) {
        LoadAsFontTexture(this->holder, renderable);
      } else {
        // No font or texture but we have uninit renderable..?
        //assert(false && "Something horrible has happened.");
      }
      renderable.isDirty = false;
    });

    // FIXME(SMA) : Measure if this is efficent, create a new 
    // vector of sorteded entity refs and draw them.
    // I suspect this is -very- inefficent.
    typedef std::pair<float,
                      std::reference_wrapper<const Renderable>> renderable_ref_pair;
    std::vector< std::vector<renderable_ref_pair> > sorted_drawable_layers;
    sorted_drawable_layers.resize(max_layers+1);
    // Resize defualt layer to expect most of the entites to sit in.
    sorted_drawable_layers[0].reserve(em.size());
    em.each<Renderable>(
      [this,dt, &sorted_drawable_layers](ex::Entity entity, Renderable &renderable) {
        // If need to sync game body to screen position.
        bool should_draw = true;
        if ( auto trans_ptr = renderable.transform ) {
          trans_ptr->setScale(renderable.scale);
          trans_ptr->setRotation(renderable.rotation);
          if ( entity.has_component<Body>() ) {
            auto& sorted_drawable = sorted_drawable_layers.at(std::min((size_t)renderable.layer,this->max_layers));
            auto bod = entity.component<Body>();
            trans_ptr->setPosition(bod->position + renderable.offset);
            const renderable_ref_pair p{ bod->position.y, std::cref(renderable) };
            // Sort drawables by 
            sorted_drawable.insert(
              std::upper_bound(sorted_drawable.begin(), sorted_drawable.end(),
                                p,
                                [](const renderable_ref_pair& lhs, const renderable_ref_pair& rhs) {
                                  return lhs.first <= rhs.first;
                                }), p
            );
            should_draw = false;
          }
        }

        // If we have animated sprite information.
        if ( auto sprite_ptr = renderable.sprite ) {
          if ( renderable.current_animation.length() != 0) {
            // Case insenstively match the first animation found by name.
            // FIXME(SMA) : string compare bottle necks the render loop.
            auto anim_iter = std::find_if(renderable.animations.begin(),
                                          renderable.animations.end(),
                                          [renderable](const auto& item) {
              return case_insensitive_string_eq(item.first, renderable.current_animation);
            });
            const auto& animation = anim_iter->second;
            // HACK(SMA): renderable.cur_frame_time is a float instead of duration.
            using FpSeconds =
              std::chrono::duration<float, std::chrono::seconds::period>;
            renderable.cur_frame_time += FpSeconds(dt).count();
            auto cur_frame_time = entityx::TimeDelta(renderable.cur_frame_time);
            size_t offset = (renderable.cur_frame >= 0) ? renderable.cur_frame
              : -renderable.cur_frame;
            // Advance animation if we're out of time for the current frame.
            if ( cur_frame_time >= animation.frame_delay[offset] ) {
              // HACK(SMA) : Some ugly hacks to support loop types.
              // Normal 'forward'
              if ( animation.loop_type == 0 ) {
                renderable.cur_frame++;
                renderable.cur_frame_time = 0.f;
                // Assume never negative here.
                assert(renderable.cur_frame > 0);
                if ( size_t(renderable.cur_frame) >= animation.frame_offsets.size() ) {
                  renderable.cur_frame = 0;
                }
              // Reverse
              } else if ( animation.loop_type == 1 ) {
                renderable.cur_frame--;
                renderable.cur_frame_time = 0.f;
                if ( renderable.cur_frame < 0 ) {
                  renderable.cur_frame = animation.frame_offsets.size() - 1;
                }
              // Ping pong.
              } else if ( animation.loop_type == 2 ) {
                renderable.cur_frame++;
                renderable.cur_frame_time = 0.f;
                if ( renderable.cur_frame > 0 &&
                    size_t(renderable.cur_frame) >= animation.frame_offsets.size() ) {
                  renderable.cur_frame = 1 - animation.frame_offsets.size();
                }
              } else {
                assert(false && "Unkown loop type specified.");
              }
            }
            if ( renderable.cur_frame >= 0 ) {
              sprite_ptr->setTextureRect(animation.frame_offsets[renderable.cur_frame]);
            } else {
              sprite_ptr->setTextureRect(animation.frame_offsets[-renderable.cur_frame]);
            }
          } else {
            renderable.cur_frame = 0;
            renderable.cur_frame_time = 0.f;
            // HACK(SMA) : If we have animations just pick the first offset we have.
            if ( renderable.animations.size() != 0 ) {
              auto& animation = renderable.animations.begin()->second;
              sprite_ptr->setTextureRect(animation.frame_offsets[0]);
            }
          }
        }

        // Draw unsorted entiteies to screen
        // In this case if it has no body its unsorted.
        if ( should_draw ) {
          if( renderable.drawable )
            this->target.draw(*renderable.drawable);
        }
      });

    for ( const auto& sorted_drawable : sorted_drawable_layers ) {
      for ( const auto& d : sorted_drawable ) {
        this->target.draw(*(d.second.get().drawable));
      }
    }
}
