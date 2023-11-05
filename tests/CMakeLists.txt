enable_testing()

function(run_bash_test TEST_NAME SCRIPT_NAME CONFIG_NAME)
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_NAME}.sh $<TARGET_FILE:membase> ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_NAME}.json
    )
endfunction()

run_bash_test(BasicTestMemory basic memory_config)
run_bash_test(ErrorsTestMemory errors memory_config)
run_bash_test(FuzzingTestMemory fuzzing memory_config)
run_bash_test(FindTestMemory find memory_config)

run_bash_test(BasicTestPersistent basic persistent_config)
run_bash_test(ErrorsTestPersistent errors persistent_config)
run_bash_test(FuzzingTestPersistent fuzzing persistent_config)
run_bash_test(FindTestPersistent find persistent_config)

add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS membase
)

