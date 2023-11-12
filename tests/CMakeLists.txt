enable_testing()

function(run_test TEST_NAME SCRIPT_NAME CONFIG_NAME)
    add_test(
        NAME ${TEST_NAME}
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_NAME}.py $<TARGET_FILE:membase> ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_NAME}.json
    )
endfunction()

run_test(BasicTestMemory basic memory_config)
run_test(ErrorsTestMemory errors memory_config)
run_test(FuzzingTestMemory fuzzing memory_config)
run_test(FindTestMemory find memory_config)
run_test(StressTestMemory stress memory_config)

run_test(BasicTestPersistent basic persistent_config)
run_test(ErrorsTestPersistent errors persistent_config)
run_test(FuzzingTestPersistent fuzzing persistent_config)
run_test(FindTestPersistent find persistent_config)

add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS membase
)

