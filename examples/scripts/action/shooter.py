import entityx
from _entityx_components import CollisionCategory, Body, InputResponder, Physics, Stats, b2BodyType
from mouse import MouseFollower

class Shooter(entityx.Entity):
    updated = False
    fireCooldown = 0.2
    curCount = 0
    is_firing = False
    size = 15
    min_size = 10.0
    max_size = 25.0

    lerp_time = 0
    init_size = 0
    lerp_time = 2

    body = entityx.Component(Body)
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        self.updated = True
        input_events = self.inresponder.responds
        if "+Use" in input_events:
            self.is_firing = True
            self.lerp_time = 0
        if "-Use" in input_events:
            self.is_firing = False

        if (self.curCount <= 0):
            if(self.is_firing):
                self.spawnMouseFollower()
                # Reset cooldown
                self.curCount = self.fireCooldown
        else:
            self.curCount -= dt;

        if ((self.is_firing == False) and (self.size != self.min_size)):
            if (self.lerp_time >= self.lerp_time):
                self.size = self.min_size
                self.lerp_time = 0
                self.lerp_size = 0
            else:
                if (self.init_size == 0):
                    self.init_size = self.size
                self.size = ((self.lerp_time / self.lerp_time) * (self.min_size - self.init_size)) + self.init_size
            self.lerp_time += dt


    def spawnMouseFollower(self):
        # TODO(SMA) : Load from JSON file.
        e = MouseFollower()
        bod = e.Component(Body)
        bod.position = self.body.position
        bod.direction = self.body.direction
        phys = e.Component(Physics)
        phys.size.x = int(self.size)
        phys.size.y = int(self.size)
        phys.category = CollisionCategory.CATEGORY_3;
        phys.mask.bits = CollisionCategory.CATEGORY_1 | CollisionCategory.CATEGORY_3 | CollisionCategory.CATEGORY_4;
        phys.bodyType = b2BodyType.DYNAMIC
        self.size = min(self.max_size, self.size + 1)
        stats = e.Component(Stats)
        stats.speed = min( 600, ( (self.max_size ) * 30) - (self.size * 15))
        e.can_explode = True

