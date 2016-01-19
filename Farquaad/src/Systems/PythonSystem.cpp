// Copyright 2016 Bablawn3d5

#include <boost/python.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Components/PythonScript.hpp>
#include <entityx/entityx.h>
#include <sstream>
#include <string>

namespace ex = entityx;
namespace py = boost::python;

PythonSystem::PythonSystem() {}
PythonSystem::~PythonSystem() {}

void PythonSystem::update(ex::EntityManager & em,
                          ex::EventManager & events, ex::TimeDelta dt) {
    em.each<PythonScript>(
        [=](ex::Entity entity, PythonScript& python) { // NOLINT
        try {
            // Access PythonEntity and call Update.
            python.object.attr("update")(dt);
        }
        catch ( const py::error_already_set& ) {
            PyErr_Print();
            PyErr_Clear();
            throw;
        }
    });
}

struct EntityToPythonEntity {
    static PyObject *convert(ex::Entity entity) {
        auto python = entity.component<PythonScript>();
        assert(python && "Entity does not have a PythonComponent");
        return py::incref(python->object.ptr());
    }
};

static std::string Entity_Id_repr(ex::Entity::Id id) {
    std::stringstream repr;
    repr << "<Entity::Id " << id.index() << "." << id.version() << ">";
    return repr.str();
}

struct PythonEntity {
    explicit PythonEntity(ex::EntityManager* em, ex::Entity::Id id) :
        _entity(ex::Entity(em, id)) {
    }
    virtual ~PythonEntity() {}

    void destroy() {
        _entity.destroy();
    }

    operator ex::Entity() const { return _entity; }

    virtual void update(ex::TimeDelta dt) {}

    ex::Entity::Id _entity_id() const {
        return _entity.id();
    }

    ex::Entity _entity;
};

static std::string PythonEntity_repr(const PythonEntity &entity) {
    std::stringstream repr;
    repr << "<Entity " << entity._entity.id().index()
        << "." << entity._entity.id().version() << ">";
    return repr.str();
}

ex::Entity::Id EntityManager_configure(ex::EntityManager& em, py::object self) {
    ex::Entity entity = em.create();
    entity.assign<PythonScript>(self);
    return entity.id();
}

/**
* A helper function for class_ to assign a component to an entity.
*/
template <typename Component>
void assign_to(Component& component, ex::EntityManager& em, ex::Entity::Id id) {
    em.assign<Component>(id, component);
}

/**
* A helper function for retrieving an existing component associated with an
* entity.
*/
template <typename Component>
Component* get_component(ex::EntityManager& em, ex::Entity::Id id) {
    return em.component<Component>(id).get();
}

BOOST_PYTHON_MODULE(_entityx) {
    py::to_python_converter<ex::Entity, EntityToPythonEntity>();
    py::implicitly_convertible<PythonEntity, ex::Entity>();

    // py::class_<BaseEvent, ptr<BaseEvent>, boost::noncopyable>("BaseEvent", py::no_init);

    py::class_<PythonEntity>("Entity", py::init<ex::EntityManager*, ex::Entity::Id>())
        .def_readonly("_entity_id", &PythonEntity::_entity_id)
        .def("update", &PythonEntity::update)
        .def("destroy", &PythonEntity::destroy)
        .def("__repr__", &PythonEntity_repr);

    py::class_<ex::Entity::Id>("EntityId", py::no_init)
        .def_readonly("id", &ex::Entity::Id::id)
        .def_readonly("index", &ex::Entity::Id::index)
        .def_readonly("version", &ex::Entity::Id::version)
        .def("__repr__", &Entity_Id_repr);

    py::class_<PythonScript>("PythonScript", py::init<py::object>())
        .def("assign_to", &assign_to<PythonScript>)
        // Return a raw pointer here.
        .def("get_component", &get_component<PythonScript>, py::return_internal_reference<>())
        .staticmethod("get_component");

    py::class_<ex::EntityManager, boost::noncopyable>("EntityManager", py::no_init)
        .def("configure", &EntityManager_configure);

    void (ex::EventManager::*emit)(const ex::BaseEvent &) = &ex::EventManager::emit;

    py::class_<ex::EventManager, boost::noncopyable >("EventManager", py::no_init)
        .def("emit", emit);
}
