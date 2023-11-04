cmake_minimum_required(VERSION 3.15)

project(membase VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(deps)
add_subdirectory(membase)
add_subdirectory(tests)

