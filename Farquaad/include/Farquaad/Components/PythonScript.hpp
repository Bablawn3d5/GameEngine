// Copyright 2016 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <entityx/entityx.h>
#include <string>

class PythonScript {
public:
    template <typename ...Args>
    PythonScript(const std::string &module, const std::string &cls, Args ... args) :
        module(module), cls(cls) {
        unpack_args(args...);
    }

    explicit PythonScript(boost::python::object object) : object(object) {}

    boost::python::object object;
    boost::python::list args;
    const std::string module, cls;
private:
    template <typename A, typename ...Args>
    void unpack_args(A &arg, Args ... remainder) { // NOLINT
        args.append(arg);
        unpack_args(remainder...);
    }

    void unpack_args() {}
};
