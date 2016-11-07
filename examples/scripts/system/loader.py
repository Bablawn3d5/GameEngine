import entityx
from _entityx_components import Body, Renderable
from gamemath import vector2
import sys
Vector2 = vector2.Vector2

class Loader(entityx.Entity):
    def __init__(self):
        self.body = self.Component(Body)

    def update(self, dt):
        cur_pos = Vector2(self.body.position)
        sys.stdout.write('*')

