import entityx
from _entityx_components import Body, InputResponder, Physics, Stats, b2BodyType
from mouse import MouseFollower

class Shooter(entityx.Entity):
    updated = False
    fireCooldown = 0.2
    curCount = 0
    is_firing = False
    size = 15

    body = entityx.Component(Body)
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        self.updated = True

        input_events = self.inresponder.responds
        if "+Use" in input_events:
            self.is_firing = True
        if "-Use" in input_events:
            self.is_firing = False

        if (self.curCount <= 0):
            if(self.is_firing):
                self.spawnMouseFollower()
                # Reset cooldown
                self.curCount = self.fireCooldown
        else:
            self.curCount -= dt;

    def spawnMouseFollower(self):
        # TODO(SMA) : Load from JSON file.
        e = MouseFollower()
        bod = e.Component(Body)
        bod.position = self.body.position
        bod.direction = self.body.direction
        phys = e.Component(Physics)
        phys.size.x = self.size
        phys.size.y = self.size
        self.size = self.size + 1
        phys.bodyType = b2BodyType.DYNAMIC
        stats = e.Component(Stats)
        stats.speed = 100

