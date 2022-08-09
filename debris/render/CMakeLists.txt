cmake_minimum_required (VERSION 3.8)

if(WIN32)
    add_subdirectory("win32")
else()
    message(FATAL_ERROR "No renderers for target platform")
endif()
