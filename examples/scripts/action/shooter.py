import entityx
from _entityx_components import CollisionCategory, Body, InputResponder, Physics, Stats, b2BodyType
from follower import Follower
from gamemath import vector2

Vector2 = vector2.Vector2

class Shooter(entityx.Entity):
    def __init__(self):
        self.body = self.Component(Body)
        self.inresponder = self.Component(InputResponder)
        self.fireCooldown = 0.2
        self.curCount = 0
        self.is_firing = False
        self.box_size = 15
        self.min_size = 10.0
        self.max_size = 25.0
        self.lerp_time = 0
        self.init_size = 0
        self.lerp_time = 2

    def update(self, dt):
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

        if ((self.is_firing == False) and (self.box_size != self.min_size)):
            if (self.lerp_time >= self.lerp_time):
                self.box_size = self.min_size
                self.lerp_time = 0
                self.lerp_size = 0
            else:
                if (self.init_size == 0):
                    self.init_size = self.box_size
                self.box_size = ((self.lerp_time / self.lerp_time) * (self.min_size - self.init_size)) + self.init_size
            self.lerp_time += dt


    def spawnMouseFollower(self):
        # TODO(SMA) : Load from JSON file.
        e = Follower()
        e.dest = Vector2(self.inresponder.mousePos)
        bod = e.Component(Body)
        bod.position = self.body.position
        bod.direction = self.body.direction
        phys = e.Component(Physics)
        phys.size.x = int(self.box_size)
        phys.size.y = int(self.box_size)
        phys.category = CollisionCategory.CATEGORY_3;
        phys.mask.bits = CollisionCategory.CATEGORY_1 | CollisionCategory.CATEGORY_3 | CollisionCategory.CATEGORY_4;
        phys.bodyType = b2BodyType.DYNAMIC
        self.box_size = min(self.max_size, self.box_size + 1)
        stats = e.Component(Stats)
        stats.speed = min( 600, ( (self.max_size ) * 30) - (self.box_size * 15))
        e.explode.can_explode = True

