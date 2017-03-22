// Copyright 2015-2016 Bablawn3d5
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

// Filesystem
#if (defined(_WIN32) || defined(WIN32)) && defined(USE_NON_TERRIBLE_FS)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else  // Not windows
// Provide a terrible version of fs::path and fs::system_complete till
// Filesystem is implemented.
namespace fs {
class path {
public:
    path(const std::string& s = "") : s(s) {};
    path(const char* s) : s(s) {};
    const std::string string() const {
        return s;
    };

    path operator/(const std::string other) const {
        std::string f = this->remove_filename().string();
#if defined(_WIN32) || defined(WIN32)
        int pos = s.rfind('\\');
#else
        int pos = s.rfind('/');
#endif
        f = f.substr(0, pos);
        // Yolo append "/" because it works on both.
        f.append("/").append(other);
        return path(f);
    }

    path remove_filename() const {
        // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
        int pos = s.rfind('\\');
#else
        int pos = s.rfind('/');
#endif

        // Get the path and the name
        std::string fpath = s.substr(0, pos + 1);
        return path(fpath);
    }
    std::string s;
};

path system_complete(const char* p) {
    return path(p);
}
}
#endif // Endif not being terrible.


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
