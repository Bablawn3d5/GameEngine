// Copyright 2015 Bablawn3d5

#include <Farquaad/Core/ComponentSerializer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <streambuf>

const std::string ComponentSerializer::toString() const {
    std::stringstream stream;
    stream << value;
    return stream.str();
}

int ComponentSerializer::ParseEntityString(const std::string str) {
    Json::Value root;
    std::istringstream stream(str);
    try {
        stream >> root;
    }
    catch ( Json::RuntimeError e ) {
        std::cerr << "Could not parse string:" << std::endl << str << std::endl;
        return 0;
    }
    this->value = root;
    return 1;
}

int ComponentSerializer::LoadFromFile(const std::string& filename, ComponentSerializer& cs) {
    std::ifstream openfile(filename);
    if ( !openfile.is_open() ) {
        std::cerr << "Could not open file: '" << filename << "'";
        return 0;
    }
    return ComponentSerializer::LoadFromStream(openfile, cs);
}

int ComponentSerializer::LoadFromStream(const std::istream& stream, ComponentSerializer& cs) {
    std::ostringstream ssin;
    ssin << stream.rdbuf();
    std::string doc = ssin.str();
    return cs.ParseEntityString(doc);
}
