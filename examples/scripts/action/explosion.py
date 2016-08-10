import entityx
from _entityx import Entity
from _entityx_components import CollisionCategory, Destroyed, Body, Physics, Stats, b2BodyType
from gamemath import vector2

Vector2 = vector2.Vector2

class Explosion(entityx.Entity):
    body = entityx.Component(Body)
    physics = entityx.Component(Physics)
    stats = entityx.Component(Stats)
    size = 100
    current_life = 0
    explode_time = 0.75
    expand_rate = 137.5
    initd = False
    physics.isSensor  = True

    def init(self):
        self.death = self.Component(Destroyed)
        self.death.timer = self.explode_time
        self.physics.size.x    = self.size
        self.physics.size.y    = self.size
        self.physics.category  = CollisionCategory.CATEGORY_4;
        self.physics.mask.bits = CollisionCategory.CATEGORY_4 | CollisionCategory.CATEGORY_3 | CollisionCategory.CATEGORY_2;
        self.physics.dirty     = True
        self.initd = True

    def update(self, dt):
        if (self.initd == False):
            self.init()

        if self.physics.isColliding:
            # Destroy everything we catch.
            for ent in self.physics.currentCollisions:
                ent.Component(Destroyed)

        # Calculate expanded size
        if ( self.current_life+dt >= self.explode_time):
            extend_px = self.expand_rate * (self.explode_time - self.current_life)
        else:
            extend_px = self.expand_rate * dt
            self.current_life += dt

        self.size = self.size + extend_px

        # Sync new-explosion based on center
        pos = Vector2(self.body.position) - 0.5*Vector2(extend_px, extend_px)
        pos.copy_to(self.body.position)
        self.physics.size.x = int(self.size)
        self.physics.size.y = int(self.size)
        self.physics.dirty = True
