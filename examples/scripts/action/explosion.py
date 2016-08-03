import entityx
from _entityx import Entity
from _entityx_components import CollisionCategory, Destroyed, Body, Physics, Stats, b2BodyType

class Explosion(entityx.Entity):
    body = entityx.Component(Body)
    physics = entityx.Component(Physics)
    stats = entityx.Component(Stats)
    size = 100
    expanding_size = 137.5
    initd = False
    physics.isSensor  = True

    def init(self):
        print "Init'd!" + str(entityx.Entity)
        self.death = self.Component(Destroyed)
        self.death.timer = 0.75
        self.physics.size.x    = self.size
        self.physics.size.y    = self.size
        self.physics.category  = CollisionCategory.CATEGORY_4;
        self.physics.mask.bits = CollisionCategory.CATEGORY_4 | CollisionCategory.CATEGORY_3 | CollisionCategory.CATEGORY_2;
        self.physics.dirty     = True
        self.initd = True

    def update(self, dt):
        if (self.initd == False):
            self.init()
        self.size = self.size + self.expanding_size * dt
        if self.physics.isColliding:
            print "Boom!" + str(self) + " size:" + str(len(self.physics.currentCollisions))
            for ent in self.physics.currentCollisions:
                ent.Component(Destroyed)
                print "Destroying entity:" + str(ent)
        self.physics.size.x = int(self.size)
        self.physics.size.y = int(self.size)
        self.physics.dirty = True
