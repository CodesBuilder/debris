cmake_minimum_required (VERSION 3.8)

set(WITH_OPENGL_BACKEND CACHE BOOL "With OpenGL backend support")
set(WITH_VULKAN_BACKEND CACHE BOOL "With Vulkan backend support")

if(WIN32)
    set(WITH_D3D11_BACKEND FALSE CACHE BOOL "With Direct3D 11 backend support")

    add_subdirectory("win32")
else()
    message(FATAL_ERROR "No native layer for target platform")
endif()
