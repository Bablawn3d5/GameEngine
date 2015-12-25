cmake_minimum_required(VERSION 3.1)

project(jsoncpp-amalgamate)

include_directories(${PROJECT_SOURCE_DIR})
add_library(jsoncpp-amalgamate STATIC jsoncpp.cpp )
