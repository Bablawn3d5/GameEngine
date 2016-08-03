import entityx
from _entityx_components import Body, InputResponder, Physics, Stats
from gamemath import vector2
from explosion import Explosion

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
                bod = e.Component(Body)
                phys = e.Component(Physics)
                pos = Vector2(self.body.position) - 0.5*Vector2(e.size, e.size)
                pos.copy_to(bod.position)
