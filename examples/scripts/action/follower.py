import entityx
from _entityx_components import Body, Physics, Stats
from explosion import Exploder, Explodes
from gamemath import vector2

Vector2 = vector2.Vector2

class Follower(entityx.Entity):
    def __init__(self):
        self.body = self.Component(Body)
        self.physics = self.Component(Physics)
        self.explode = Explodes()
        self.dest = None

    def update(self, dt):
        cur_pos = Vector2(self.body.position)
        dest_pos = self.dest

        direction_vec = dest_pos - cur_pos
        direction_vec.normalize()

        direction_vec.copy_to(self.body.direction)
        Exploder.check_explodes(self, dt)
