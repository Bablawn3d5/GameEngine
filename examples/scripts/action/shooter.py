import entityx
from _entityx_components import Body

class AnEntity(Entity):
    updated = False
    frameCount = 10
    curCount = frameCount
    body = entityx.Component(Body)

    def emit_collision_from_python():
        # a = AnEntity()
        # b = AnEntity()
        # collision = Collision(a, b)
        emit(collision)

    def update(self, dt):
        self.updated = True
        self.curCount -= 1;
        if(self.curCount == 0):
            e = entityx.Entity()
            bod = e.Component(Body)
            bod.position = self.body.position
            bod.direction = self.body.direction
            self.curCount = self.frameCount

