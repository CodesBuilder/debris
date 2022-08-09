cmake_minimum_required (VERSION 3.8)

include_directories(${PROJECT_SOURCE_DIR})
add_library (debris-native STATIC "window.h" "window.cpp")
