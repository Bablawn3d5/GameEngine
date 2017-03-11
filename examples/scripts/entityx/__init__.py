import _entityx
import json, operator

"""These classes provide a convenience layer on top of the raw entityx::python
primitives.

They allow you to declare your entities and components in an intuitive way:

    class Player(Entity):
        position = Component(Position)
        direction = Component(Direction)
        sprite = Component(Sprite, 'player.png')  # Sprite component with constructor argument

        def update(self, dt, frame):
            self.position.x += self.direction.x * dt
            self.position.x += self.direction.y * dt

Note that components assigned from C++ must be assigned prior to assigning
PythonComponent, otherwise they will be created by the Entity constructor.
"""


__all__ = ['Entity', 'Component']


class Component(object):
    """A field that manages Component creation/retrieval.

    Use like so:

    class Player(Entity):
        position = Component(Position)

        def move_to(self, x, y):
            self.position.x = x
            self.position.y = y
    """
    def __init__(self, cls, *args, **kwargs):
        self._cls = cls
        self._args = args
        self._kwargs = kwargs

    def _build(self, entity_id):
        component = self._cls.get_component(_entityx._entity_manager, entity_id)
        if not component:
            component = self._cls(*self._args, **self._kwargs)
            component.assign_to(_entityx._entity_manager, entity_id)
            return self._build(entity_id)
        return component


class EntityMetaClass(object.__class__):
    """Collect registered components from class attributes.

    This is done at class creation time to reduce entity creation overhead.
    """
    
    def __new__(cls, name, bases, dct):
        dct['_components'] = components = {}
        # Collect components from base classes
        for base in bases:
            if '_components' in base.__dict__:
                components.update(base.__dict__['_components'])
        # Collect components
        for key, value in dct.items():
            if isinstance(value, Component):
                components[key] = value
        return type.__new__(cls, name, bases, dct)

class Entity(object):
    """Base Entity class.

    Python Enitities differ in semantics from C++ components, in that they
    contain logic, and so on.
    """
    __metaclass__ = EntityMetaClass

    def __new__(cls, *args, **kwargs):
        entity = kwargs.pop('entity', None)
        self = object.__new__(cls, *args, **kwargs)
        if entity is None:
            entity = _entityx._entity_manager.new_entity(self)
        # Initalize self.entity 
        self.entity = entity
        cls.__init__(self, *args, **kwargs)
        for k, v in self._components.items():
            setattr(self, k, v._build(self.entity.id))
        return self

    def __init__(self):
        """Default constructor."""

    def __repr__(self):
        return '<%s.%s %s.%s>' % (self.__class__.__module__, self.__class__.__name__, self.entity.id.index, self.entity.id.version)

    @property
    def id(self):
        return self.entity.id

    def destroy(self):
        return self.entity.destroy()

    ''' For seralization '''
    def to_json(self):
        ''' Gets all vars including C++ varaibles of a class''' 
        def get_vars(obj):
            attrs = [attr for attr in dir(obj) if not callable(getattr(obj, attr)) and not attr.startswith("_")]
            return attrs
        ''' Fake equiablient of __dict__ that also includes C++ classes '''
        def get_dict(obj):
            attrs = get_vars(obj)
            dic = {}
            for member in attrs:
                att = getattr(obj,member)
                # End recursion if C++ object provides __repr__ 
                if type(att).__repr__ != object.__repr__:
                    dic[member] = str(att)
                # Recursively go over components that don't provide __repr__
                else:
                    dic[member] = get_dict(att)
            return dic
        return json.dumps(get_dict(self), indent=2)

    @classmethod
    def _from_raw_entity(cls, *args, **kwargs):
        """Create a new Entity from a raw entity.

        This is called from C++.
        """
        args = list(*args)
        kwargs = dict(kwargs)
        entity = kwargs.pop('entity', None)
        self = Entity.__new__(cls, *args, entity=entity)
        cls.__init__(self, *args, **kwargs)
        return self

    '''
        Create a component if its not craeted, return component otherwise. 
    '''
    def Component(self, cls, *args, **kwargs):
        component = cls.get_component(_entityx._entity_manager, self.entity.id)
        if not component:
            component = cls(*args, **kwargs)
            component.assign_to(_entityx._entity_manager, self.entity.id)
            return self.Component(cls, *args, **kwargs)
        return component

    '''
        Returns true if the entity contains the component
    '''
    def HasComponent(self, cls):
        component = cls.get_component(_entityx._entity_manager, self.entity.id)
        if component:
            return True
        return False