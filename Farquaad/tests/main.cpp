// Copyright 2015-2016 Bablawn3d5
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// HACK(SMA) : Ugly hack to define a global and pass though to tests.
// Normally a application should be provided paths to load for python libs.
// but  I don't know how to do that with the catch runner.
fs::path execute_dir;
const fs::path get_execute_dir() {
    return execute_dir;
}

int main(int argc, char* const argv[]) {
    // global setup..
    execute_dir = fs::system_complete(argv[0]).remove_filename();

    int result = Catch::Session().run(argc, argv);
    return result;
}
