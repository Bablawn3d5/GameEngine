import entityx
from _entityx_components import Body, InputResponder, Physics, Stats

class MouseActor(entityx.Entity):
    body = entityx.Component(Body)
    physics = entityx.Component(Physics)
    inresponder = entityx.Component(InputResponder)

    def update(self, dt):
        # Convert int to float. :\
        self.body.position.x = self.inresponder.mousePos.x
        self.body.position.y = self.inresponder.mousePos.y
        self.physics.dirty = True

