// Copyright 2015 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <Farquaad/Components/PythonScript.hpp>
#include <boost/filesystem.hpp>
#include <entityx/entityx.h>
#include <string>
#include <vector>

namespace ex = entityx;
namespace fs = boost::filesystem;
namespace py = boost::python;

struct PythonSystem : public ex::System<PythonSystem>, public ex::Receiver<PythonSystem> {
public:
    explicit PythonSystem(ex::EntityManager*, const fs::path&);
    ~PythonSystem();

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt) override; // NOLINT
    void add_path(const std::string &path);

    const std::vector<std::string>& python_paths() const {
        return py_paths;
    }

    void configure(ex::EventManager& event_manager) override;
    void receive(const ex::ComponentAddedEvent<PythonScript> &event);

private:
    void initialize_python_module();

    ex::EntityManager* em;
    std::vector<std::string> py_paths;
    static bool intialized;
};

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
static Component* get_component(ex::EntityManager& em, ex::Entity::Id id) {
    auto handle = em.component<Component>(id);
    if ( !handle )
        return NULL;
    return handle.get();
}


struct EntityToPythonEntity {
  static PyObject *convert(ex::Entity entity) {
    auto python = entity.component<PythonScript>();
    assert(python && "Entity does not have a PythonComponent");
    return py::incref(python->object.ptr());
  }
};

struct PythonEntity {
  explicit PythonEntity(ex::Entity ent) :
    _entity(ent) {
    assert(_entity.valid());
  }

  explicit PythonEntity(ex::EntityManager* em, ex::Entity::Id id) :
    _entity(ex::Entity(em, id)) {
    assert(_entity.valid());
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
