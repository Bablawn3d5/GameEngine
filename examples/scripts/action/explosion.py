import entityx
from _entityx import Entity
from _entityx_components import Sound, CollisionCategory, Destroyed, Body, Physics, Stats, b2BodyType
from gamemath import vector2

Vector2 = vector2.Vector2

class Explodes():
    def __init__(self):
        self.can_explode = True
        self.explode_size = 0
        self.current_life = 0
        self.explode_time = 0.75
        self.expand_rate = 137.5

class Exploder(entityx.Entity):
    def __init__(self):
        self.body = self.Component(Body)
        self.physics = self.Component(Physics)
        self.explode = Explodes()
        self.dest = None

    @staticmethod
    def check_explodes(self, dt):
        # Explode if literally touching anything
        if self.explode.can_explode:
            if self.physics.isColliding:
                self.explode.can_explode = False
                e = Explosion();
                e.explode.explode_time = 0.3
                e.explode.expand_rate = 500
                e.explode.explode_size = 10
                bod = e.Component(Body)
                phys = e.Component(Physics)
                phys.size.x = 0
                phys.size.y = 0
                phys.isDirty = True
                pos = Vector2(self.body.position) - 0.5*Vector2(e.explode.explode_size, e.explode.explode_size)
                pos.copy_to(bod.position)
                sound = e.Component(Sound)
                sound.name = "sounds/Explode.wav"


class Explosion(entityx.Entity):
    def __init__(self):
        self.body = self.Component(Body)
        self.physics = self.Component(Physics)
        self.stats = self.Component(Stats)
        self.explode = Explodes()
        self.initd = False
        self.physics.isSensor  = True
        self.physics.size.x    = 0
        self.physics.size.y    = 0
        self.physics.category  = CollisionCategory.CATEGORY_4;
        self.physics.mask.bits = CollisionCategory.CATEGORY_4 | CollisionCategory.CATEGORY_3 | CollisionCategory.CATEGORY_2;
        self.physics.dirty     = True

    def init(self):
        self.death = self.Component(Destroyed)
        self.death.timer = self.explode.explode_time
        self.initd = True


    def update(self, dt):
        # Start timer on first frame
        if (self.initd == False):
            self.init()

        if self.physics.isColliding:
            # Destroy everything we catch.
            for ent in self.physics.currentCollisions:
                ent.Component(Destroyed)

        # Calculate expanded size
        if ( self.explode.current_life+dt >= self.explode.explode_time):
            extend_px = self.explode.expand_rate * (self.explode.explode_time - self.explode.current_life)
        else:
            extend_px = self.explode.expand_rate * dt
            self.explode.current_life += dt

        self.explode.explode_size = self.explode.explode_size + extend_px

        # Sync new-explosion based on center
        pos = Vector2(self.body.position) - 0.5*Vector2(extend_px, extend_px)
        pos.copy_to(self.body.position)
        self.physics.size.x = int(self.explode.explode_size)
        self.physics.size.y = int(self.explode.explode_size)
        self.physics.dirty = True
