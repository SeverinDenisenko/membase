if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(MEMBASE_COMPILE_OPTIONS "-Wall" "-Wextra" "-Werror" "-fsanitize=address" "-Wno-unknown-warning-option" "-Wno-error=class-memaccess")
    set(MEMBASE_LINK_OPTIONS "-fsanitize=address")
else()
    set(MEMBASE_COMPILE_OPTIONS "-Wall" "-Wextra" "-Werror" "-Wno-unknown-warning-option" "-Wno-error=class-memaccess")
    set(MEMBASE_LINK_OPTIONS)
endif()

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-Bstatic")

file(GLOB LIB_SRC lib/*)
add_library(membase-lib STATIC ${LIB_SRC})
set(MEMBASE_LIB_INCLUDE lib/include)
target_compile_options(membase-lib PRIVATE ${MEMBASE_COMPILE_OPTIONS})
target_link_options(membase-lib PRIVATE ${MEMBASE_LINK_OPTIONS})
target_include_directories(membase-lib PUBLIC ${MEMBASE_LIB_INCLUDE})
target_link_libraries(membase-lib PUBLIC
    gflags
    glog
    nlohmann_json
    fmt
    leveldb
    Boost::asio
    Boost::system
    Boost::thread
)

file(GLOB APP_SRC app/*)
add_executable(membase ${APP_SRC})
target_compile_options(membase PRIVATE ${MEMBASE_COMPILE_OPTIONS})
target_link_options(membase PRIVATE ${MEMBASE_LINK_OPTIONS})
target_include_directories(membase-lib PUBLIC ${MEMBASE_LIB_INCLUDE})
target_link_libraries(membase
    membase-lib
)

