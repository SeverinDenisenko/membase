cmake_minimum_required(VERSION 3.15)

project(membase VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(deps/gflags)
set(WITH_GFLAGS NO)
add_subdirectory(deps/glog)
add_subdirectory(deps/json)
add_subdirectory(deps/fmt)
add_subdirectory(deps/boost)

file(GLOB SRC src/*)
add_executable(membase ${SRC})
target_compile_options(membase PUBLIC "-Wall" "-Wextra" "-Werror")
target_link_libraries(membase
    gflags
    glog
    nlohmann_json
    fmt
    Boost::asio
    Boost::system
    Boost::thread)

add_subdirectory(tests)
