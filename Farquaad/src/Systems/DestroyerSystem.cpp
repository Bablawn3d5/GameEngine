// Copyright 2016 Bablawn3d5

#include <Farquaad/Systems/DestroyerSystem.h>
#include <Farquaad/Components/Destroyed.hpp>

// HACK(SMA) : Forward declare these components as we really don't care about
// lvalue use.
struct Physics;
struct PythonScript;

DestroyerSystem::DestroyerSystem() {}

void DestroyerSystem::update(ex::EntityManager & em,
  ex::EventManager & events, ex::TimeDelta dt) {
  // Do stuff with inputs
  em.each<Destroyed>(
    [&em, &dt](ex::Entity entity, Destroyed &d) {
        d.deathTimer -= static_cast<float>(dt.count());
        if ( d.deathTimer <= 0 ) { 
          entity.destroy(); 
        }
  });
}
