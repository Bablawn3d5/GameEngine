// Copyright 2016 Bablawn3d5
// NOTE: MUST be first include. See http://docs.python.org/2/extending/extending.html
#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Components/PythonScript.hpp>
#include <entityx/entityx.h>
#include <catch.hpp>
#include <Common.h>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

namespace fs = boost::filesystem;
namespace py = boost::python;
using std::cerr;
using std::endl;
using namespace entityx;

BOOST_PYTHON_MODULE(entityx_python_test) {
    py::class_<Position>("Position", py::init<py::optional<float, float>>())
        .def("assign_to", &assign_to<Position>)
        .def("get_component", &get_component<Position>,
             py::return_value_policy<py::reference_existing_object>())
        .staticmethod("get_component")
        .def_readwrite("x", &Position::x)
        .def_readwrite("y", &Position::y);

    py::class_<Direction>("Direction", py::init<py::optional<float, float>>())
        .def("assign_to", &assign_to<Direction>)
        .def("get_component", &get_component<Direction>,
             py::return_value_policy<py::reference_existing_object>())
        .staticmethod("get_component")
        .def_readwrite("x", &Direction::x)
        .def_readwrite("y", &Direction::y);
}

// Get global executable path from main.cpp
extern const fs::path get_execute_dir();

class PythonSystemTest {
protected:
    PythonSystemTest() : em(ev) {
        assert(PyImport_AppendInittab("entityx_python_test", initentityx_python_test) != -1
               && "Failed to initialize entityx_python_test Python module");
        fs::path exec_dir = get_execute_dir();
        // HACK(SMA) : Assuming python27.zip contains our python std_lib and in in our exec dir.
        // normally this is passed in by the application.
        python = std::make_shared<PythonSystem>(&em, exec_dir);

        // Assuming test scripts are also avaibile from this directory
        fs::path script_dir = get_execute_dir();
        script_dir += "/scripts/";
        // HACK(SMA) : Droping wide characters here
        python->add_path(script_dir.string());
        python->configure(ev);

        if ( !initialized ) {
            initentityx_python_test();
            initialized = true;
        }
    }

    EventManager ev;
    EntityManager em;
    std::shared_ptr<PythonSystem> python;
    static bool initialized;
};

bool PythonSystemTest::initialized = false;

TEST_CASE_METHOD(PythonSystemTest, "TestSystemUpdateCallsEntityUpdate") {
    REQUIRE(em.size() == 0UL);
    try {
        Entity e = em.create();
        auto script = e.assign<PythonScript>("entityx.tests.update_test", "UpdateTest");
        REQUIRE(!py::extract<bool>(script->object.attr("updated")));
        python->update(em, ev, 0.1);
        REQUIRE(py::extract<bool>(script->object.attr("updated")));
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}

TEST_CASE_METHOD(PythonSystemTest, "TestComponentAssignmentCreationInPython") {
    REQUIRE(em.size() == 0UL);
    try {
        Entity e = em.create();
        REQUIRE(e.valid());
        auto script = e.assign<PythonScript>("entityx.tests.assign_test", "AssignTest");
        REQUIRE(static_cast<bool>(e.component<Position>()));
        REQUIRE(script->object);
        REQUIRE(script->object.attr("test_assign_create"));
        script->object.attr("test_assign_create")();
        auto position = e.component<Position>();
        REQUIRE(static_cast<bool>(position));
        REQUIRE(position->x == 1.0);
        REQUIRE(position->y == 2.0);
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}

TEST_CASE_METHOD(PythonSystemTest, "TestComponentAssignmentCreationInCpp") {
    REQUIRE(em.size() == 0UL);
    try {
        Entity e = em.create();
        e.assign<Position>(2.f, 3.f);
        auto script = e.assign<PythonScript>("entityx.tests.assign_test", "AssignTest");
        REQUIRE(static_cast<bool>(e.component<Position>()));
        REQUIRE(script->object);
        REQUIRE(script->object.attr("test_assign_existing"));
        script->object.attr("test_assign_existing")();
        auto position = e.component<Position>();
        REQUIRE(static_cast<bool>(position));
        REQUIRE(position->x == 3.0);
        REQUIRE(position->y == 4.0);
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}

TEST_CASE_METHOD(PythonSystemTest, "TestEntityConstructorArgs") {
    REQUIRE(em.size() == 0UL);
    try {
        Entity e = em.create();
        e.assign<PythonScript>("entityx.tests.constructor_test", "ConstructorTest", 4.0, 5.0);
        auto position = e.component<Position>();
        REQUIRE(static_cast<bool>(position));
        REQUIRE(position->x == 4.0);
        REQUIRE(position->y == 5.0);
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}

TEST_CASE_METHOD(PythonSystemTest, "TestDeepEntitySubclass") {
    REQUIRE(em.size() == 0UL);
    try {
        Entity e = em.create();
        auto script = e.assign<PythonScript>("entityx.tests.deep_subclass_test", "DeepSubclassTest");
        REQUIRE(script->object.attr("test_deep_subclass"));
        script->object.attr("test_deep_subclass")();

        Entity e2 = em.create();
        auto script2 = e2.assign<PythonScript>("entityx.tests.deep_subclass_test", "DeepSubclassTest2");
        REQUIRE(script2->object.attr("test_deeper_subclass"));
        script2->object.attr("test_deeper_subclass")();
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}

TEST_CASE_METHOD(PythonSystemTest, "TestEntityCreationFromPython") {
        try {
        REQUIRE(em.size() == 0UL);
        py::object test = py::import("entityx.tests.create_entities_from_python_test");
        test.attr("create_entities_from_python_test")();
        REQUIRE(em.size() == 3UL);
    }
    catch ( ... ) {
        PyErr_Print();
        PyErr_Clear();
        REQUIRE(false);
    }
}
