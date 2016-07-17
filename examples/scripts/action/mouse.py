import entityx
from _entityx_components import Body, InputResponder, Physics, Stats
from gamemath import vector2

Vector2 = vector2.Vector2

class MouseFollower(entityx.Entity):
    body = entityx.Component(Body)
    physics = entityx.Component(Physics)
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        cur_pos = Vector2(self.body.position)
        dest_pos = Vector2(self.inresponder.mousePos)

        direction_vec = dest_pos - cur_pos
        direction_vec.normalize()

        direction_vec.copy_to(self.body.direction)
        self.physics.dirty = True
