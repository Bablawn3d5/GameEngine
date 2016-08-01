// Copyright 2016 Bablawn3d5
#include <Farquaad/Systems/SpriteRenderSystem.h>
#include <Farquaad/Components.hpp>
#include <iostream>

void SpriteRenderSystem::update(ex::EntityManager & em,
                          ex::EventManager & events, ex::TimeDelta dt) {
    // Update drawables.
    em.each<Body, Renderable>(
        [this](ex::Entity entity, Body &body, Renderable &renderable) {
      // Resource exists, just exit it.
      if( renderable.drawable != nullptr )
        return;

      // Create sprite from texture
      if ( renderable.texture_name.length() != 0 ) {
        try {
          static uint32 id = 0;
          auto& texture = 
            this->texture_holder.acquire(renderable.texture_name,
                                         thor::Resources::fromFile<sf::Texture>(renderable.texture_name),
                                         thor::Resources::Reuse);

          std::string s = "entityx_" + std::to_string(id++);
          auto sprite = std::make_shared<sf::Sprite>();
          sprite->setTexture(texture);
          sprite->setPosition(body.position);
          renderable.drawable = std::static_pointer_cast<sf::Drawable>(sprite);
          renderable.transform = std::static_pointer_cast<sf::Transformable>(sprite);
        }

        // Failed to load it for whatever reason
        catch ( thor::ResourceAccessException& e ) {
          std::cerr << "Failed to load resource: " << renderable.texture_name
            << " " << e.what() << std::endl;
          throw e;
        }
      }

    });

    em.each<Renderable>(
      [this](ex::Entity entity, Renderable &renderable) {
        try {
          if ( auto trans_ptr = renderable.transform.lock() ) {
            if ( entity.has_component<Body>() ) {
              trans_ptr->setPosition(entity.component<Body>()->position);
            }
          }
          this->target.draw(*renderable.drawable);
        }
        catch ( std::exception& e ) {
          std::cerr << "Failed to load drawable. "
            <<  " " << e.what() << std::endl; 
          throw e;
        }
    });
}
