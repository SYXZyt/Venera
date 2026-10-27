cmake_minimum_required(VERSION 3.20)
project(Venera)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.44")
    message(FATAL_ERROR
        "Venera requires MSVC toolset 14.44 or newer (found cl.exe ${CMAKE_CXX_COMPILER_VERSION}, "
        "which corresponds to an older toolset). Reconfigure with toolset 14.44+ active.")
endif()

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(venera)