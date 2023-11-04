enable_testing()

add_test(
    NAME SanityTest
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/sanity.sh $<TARGET_FILE:membase> ${CMAKE_CURRENT_SOURCE_DIR}/config.json
)

add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS membase
)

