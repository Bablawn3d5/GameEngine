// Copyright 2015 Bablawn3d5

#include <Farquaad/Core/ComponentSerializer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <streambuf>

std::string ComponentSerializer::toString() {
    std::stringstream stream;
    stream << value;
    return stream.str();
}

int ComponentSerializer::ParseEntityString(const std::string str) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(str.c_str(), root);
    if ( !parsingSuccessful ) {
        std::cout << "Failed to parse string"
            << reader.getFormattedErrorMessages();
        return 0;
    }
    this->value = root;
    return 1;
}

int ComponentSerializer::LoadFromFile(const std::string & filename, ComponentSerializer& cs) {
    std::ifstream inputfilestream(filename);
    std::string str;

    // Allocate memory upfront
    inputfilestream.seekg(0, std::ios::end);
    str.reserve(static_cast<size_t>(inputfilestream.tellg()));
    inputfilestream.seekg(0, std::ios::beg);

    inputfilestream >> cs.value;

    return 0;
}
