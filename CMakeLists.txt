cmake_minimum_required(VERSION 3.20)
project(Venera)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(venera)