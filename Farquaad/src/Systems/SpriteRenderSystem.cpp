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
void LoadAsTexture(thor::ResourceHolder<TextureWithOffsets, std::string>& texture_holder, Renderable &renderable) {
  try {
    const auto& loadingFunc = [&renderable]() {
      auto tex_obj = std::make_unique<TextureWithOffsets>();
      tex_obj->tex = *thor::Resources::fromFile<sf::Texture>(renderable.texture_name).load();
      return tex_obj;
    };
    auto& texture = texture_holder.acquire(renderable.texture_name, 
                                           thor::ResourceLoader<TextureWithOffsets>
                                          (loadingFunc, renderable.texture_name),
                                           thor::Resources::Reuse);

    auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(texture.tex);
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(sprite);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(sprite).get();
  }

  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << renderable.texture_name
      << " " << e.what() << std::endl;
    throw e;
  }
}

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsSpriteSheetTexture(thor::ResourceHolder<TextureWithOffsets, std::string>& texture_holder, Renderable &renderable) {
  try {
    // TODO(SMA): Move me to a resource loader.
    const auto& loadingFunc = [&renderable]() {
      auto tex_obj = std::make_unique<TextureWithOffsets>();
      auto assprite = aseprite::load_sprite_from_file(renderable.texture_name.c_str());
      tex_obj->tex.loadFromImage(aseprite::convertToSFML(assprite));
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
        std::vector<sf::IntRect> anim_offsets(offsets.begin() + tag.from, offsets.begin() + tag.to);
        // Copy frame delay data
        std::vector<entityx::TimeDelta> anim_delays;
        assert(anim_offsets.size() > 0);
        anim_delays.resize(anim_offsets.size());
        std::transform(assprite.frames.begin() + tag.from, assprite.frames.begin() + tag.to,
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

    auto& texture = texture_holder.acquire(renderable.texture_name,
                                          thor::ResourceLoader<TextureWithOffsets>
                                           (loadingFunc, renderable.texture_name),
                                          thor::Resources::Reuse);
    auto sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(texture.tex);
    renderable.animations = texture.anims;
    // HACK (SMA) : Set current animation to be the first frame
    // if its unset
    if ( renderable.current_animation.length() == 0 && renderable.animations.size() != 0) {
      renderable.current_animation = renderable.animations.begin()->first;
    }
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(sprite);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(sprite).get();
    renderable.sprite = std::static_pointer_cast<sf::Sprite>(sprite).get();
  }

  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << renderable.texture_name
      << " " << e.what() << std::endl;
    throw e;
  }

} // Anon Namespace

// TODO(SMA): Move me to my own managed resource reloader, maybe avoid std::shared_ptr???
void LoadAsFontTexture(thor::ResourceHolder<sf::Font, std::string>& font_holder, Renderable &renderable) {
  try {
    auto& font = font_holder.acquire(renderable.font_name,
                                thor::Resources::fromFile<sf::Font>(renderable.font_name),
                                thor::Resources::Reuse);

    auto text = std::make_shared<sf::Text>(renderable.font_string, font, renderable.font_size);
    text->setFillColor(sf::Color(renderable.r, renderable.g, renderable.b, renderable.a));
    renderable.drawable = std::static_pointer_cast<sf::Drawable>(text);
    renderable.transform = std::static_pointer_cast<sf::Transformable>(text).get();
  }
  // Failed to load it for whatever reason
  catch ( thor::ResourceAccessException& e ) {
    std::cerr << "Failed to load resource: " << renderable.texture_name
      << " " << e.what() << std::endl;
    throw e;
  }
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
          LoadAsSpriteSheetTexture(this->texture_holder, renderable);
        } else {
          LoadAsTexture(this->texture_holder, renderable);
        }
      } else if ( renderable.font_name.length() != 0 ) {
        LoadAsFontTexture(this->font_holder, renderable);
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
            renderable.cur_frame_time += dt;
            size_t offset = (renderable.cur_frame >= 0) ? renderable.cur_frame
              : -renderable.cur_frame;
            // Advance animation if we're out of time for the current frame.
            if ( renderable.cur_frame_time >= animation.frame_delay[offset] ) {
              // HACK(SMA) : Some ugly hacks to support loop types.
              // Normal 'forward'
              if ( animation.loop_type == 0 ) {
                renderable.cur_frame++;
                renderable.cur_frame_time = 0;
                // Assume never negative here.
                assert(renderable.cur_frame > 0);
                if ( size_t(renderable.cur_frame) >= animation.frame_offsets.size() ) {
                  renderable.cur_frame = 0;
                }
              // Reverse
              } else if ( animation.loop_type == 1 ) {
                renderable.cur_frame--;
                renderable.cur_frame_time = 0;
                if ( renderable.cur_frame < 0 ) {
                  renderable.cur_frame = animation.frame_offsets.size() - 1;
                }
              // Ping pong.
              } else if ( animation.loop_type == 2 ) {
                renderable.cur_frame++;
                renderable.cur_frame_time = 0;
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
            renderable.cur_frame_time = 0;
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
          this->target.draw(*renderable.drawable);
        }
      });

    for ( const auto& sorted_drawable : sorted_drawable_layers ) {
      for ( const auto& d : sorted_drawable ) {
        this->target.draw(*(d.second.get().drawable));
      }
    }
}
