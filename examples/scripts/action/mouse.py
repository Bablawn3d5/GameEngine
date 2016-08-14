import entityx
from _entityx_components import InputResponder
from gamemath import vector2
from follower import Follower

Vector2 = vector2.Vector2

class MouseFollower(Follower):
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        self.dest = Vector2(self.inresponder.mousePos)
        super(MouseFollower, self).update(dt)
