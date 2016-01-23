// Copyright 2016 Bablawn3d5

#include <boost/python.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Components/PythonScript.hpp>
#include <entityx/entityx.h>
#include <sstream>
#include <string>

namespace ex = entityx;
namespace py = boost::python;

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

void PythonSystem::add_path(const std::string &path) {
    py_paths.push_back(path);
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
        .def("get_component", &get_component<PythonScript>,
             py::return_value_policy<py::reference_existing_object>())
        .staticmethod("get_component");

    py::class_<ex::EntityManager, boost::noncopyable>("EntityManager", py::no_init)
        .def("configure", &EntityManager_configure);

    void (ex::EventManager::*emit)(const ex::BaseEvent &) = &ex::EventManager::emit;

    py::class_<ex::EventManager, boost::noncopyable >("EventManager", py::no_init)
        .def("emit", emit);
}

void PythonSystem::initialize_python_module() {
    assert(PyImport_AppendInittab("_entityx", init_entityx) != -1 && "Failed to initialize _entityx Python module");
}

bool PythonSystem::intialized = false;

PythonSystem::PythonSystem(ex::EntityManager* em, const fs::path& p) : em(em) {
    if ( !intialized ) {
        initialize_python_module();
    }
    // HACK(SMA) : Throw away wide characters
    std::string str = p.string();
    // Write to tempoaray char[] for PySys_SetPath
    std::vector<char> lib_paths(str.begin(), str.end());
    lib_paths.push_back('\0');

    // Set path so we can find the python27.zip
    Py_NoSiteFlag = 1;
    Py_SetPythonHome(&lib_paths[0]);
    Py_InitializeEx(0);

    // Assuming python27.zip is our python lib zip.
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path = ['.','python27.zip']");

    if ( !intialized ) {
        init_entityx();
        intialized = true;
    }
}

PythonSystem::~PythonSystem() {
    try {
        py::object entityx = py::import("_entityx");
        entityx.attr("_entity_manager").del();
        entityx.attr("_event_manager").del();
        py::object sys = py::import("sys");
        py::object gc = py::import("gc");
        gc.attr("collect")();
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        throw;
    }
}

void PythonSystem::configure(ex::EventManager& ev) {
    ev.subscribe<ex::ComponentAddedEvent<PythonScript>>(*this);

    try {
        py::object main_module = py::import("__main__");
        py::object main_namespace = main_module.attr("__dict__");

        py::object sys = py::import("sys");

        // Add paths to interpreter sys.path
        for ( auto path : py_paths ) {
            py::str dir = path.c_str();
            sys.attr("path").attr("insert")(0, dir);
        }

        py::object entityx = py::import("_entityx");
        entityx.attr("_entity_manager") = boost::ref(em);
        entityx.attr("_event_manager") = boost::ref(ev);
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        throw;
    }
}

void PythonSystem::receive(const ex::ComponentAddedEvent<PythonScript> &event) {
    // If the component was created in C++ it won't have a Python object
    // associated with it. Create one.
    if ( !event.component->object ) {
        py::object module = py::import(event.component->module.c_str());
        py::object cls = module.attr(event.component->cls.c_str());
        py::object from_raw_entity = cls.attr("_from_raw_entity");
        if ( py::len(event.component->args) == 0 ) {
            ex::ComponentHandle<PythonScript> p = event.component;
            p->object = from_raw_entity(event.entity.id());
        } else {
            py::list args;
            args.append(event.entity.id());
            args.extend(event.component->args);
            ex::ComponentHandle<PythonScript> p = event.component;
            p->object = from_raw_entity(*py::tuple(args));
        }
    }
}
