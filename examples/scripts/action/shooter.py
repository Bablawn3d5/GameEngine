import entityx
from _entityx_components import Body, InputResponder, Physics, Stats

class Shooter(entityx.Entity):
    updated = False
    fireRate = 0.2
    curCount = fireRate
    is_firing = False

    body = entityx.Component(Body)
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        self.updated = True

        if "+Use" in self.inresponder.responds:
            self.is_firing = True
        if "-Use" in self.inresponder.responds:
            self.is_firing = False

        if(len(self.inresponder.responds) > 0):
            for a in self.inresponder.responds:
                print a,
            else:
                print

        if(self.curCount <= 0):
            if(self.is_firing):
                # TODO(SMA) : Load from JSON file.
                e = entityx.Entity()
                bod = e.Component(Body)
                bod.position = self.body.position
                bod.direction = self.body.direction
                phys = e.Component(Physics)
                phys.size.x = 5
                phys.size.y = 5
                stats = e.Component(Stats)
                # Reset cooldown
                self.curCount = self.fireRate
        else:
            self.curCount -= dt;

