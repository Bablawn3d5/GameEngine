// Copyright 2016 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <entityx/entityx.h>
#include <string>

struct PythonScript {

    explicit PythonScript(boost::python::object object) : object(object) {}
    explicit PythonScript(const std::string &module = "", const std::string &cls = "") :
      module(module), cls(cls) {}

    template <typename ...Args>
    explicit PythonScript(const std::string &module, const std::string &cls, Args ... args) :
        module(module), cls(cls) {
        unpack_args(args...);
    }

    ~PythonScript() {}

    boost::python::object object;
    boost::python::list args;
    // HACK(SMA): This should be const
    // but then we don't get a copy constructor.
    // :((((
    //const std::string module, cls;
    std::string module, cls;

    template <typename A, typename ...Args>
    void unpack_args(A &arg, Args ... remainder) { // NOLINT
        args.append(arg);
        unpack_args(remainder...);
    }

    void unpack_args() {}
};
