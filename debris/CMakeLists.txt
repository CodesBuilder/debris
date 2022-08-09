cmake_minimum_required (VERSION 3.8)

add_library (
    debris
    STATIC
    "math.hpp"
    

    "color.h"
    "color.cpp"

    "element.h"
    "element.cpp"

    "render.h"
    "render.cpp"

    "window.h"
    "window.cpp"

    "debris.h"
    "debris.cpp"

    "renderers.h"
    "render/dx11.h" )
target_link_libraries(debris debris-native debris-renderer)

add_subdirectory("native")
add_subdirectory("render")

