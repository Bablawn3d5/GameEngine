import entityx
from _entityx_components import Body, InputResponder, Physics, Stats
from explosion import Explosion
from gamemath import vector2

Vector2 = vector2.Vector2

class MouseFollower(entityx.Entity):
    body = entityx.Component(Body)
    physics = entityx.Component(Physics)
    inresponder = entityx.Component(InputResponder)
    can_explode = False

    def update(self, dt):
        cur_pos = Vector2(self.body.position)
        dest_pos = Vector2(self.inresponder.mousePos)

        direction_vec = dest_pos - cur_pos
        direction_vec.normalize()

        direction_vec.copy_to(self.body.direction)

        # Explode if literally touching anything
        if self.can_explode:
            if self.physics.isColliding:
                self.can_explode = False
                e = Explosion();
                e.explode_time = 0.3
                e.expand_rate = 500
                e.size = 10
                bod = e.Component(Body)
                phys = e.Component(Physics)
                phys.size.x = 0
                phys.size.y = 0
                phys.isDirty = True
                pos = Vector2(self.body.position) - 0.5*Vector2(e.size, e.size)
                pos.copy_to(bod.position)
