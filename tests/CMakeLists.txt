enable_testing()

function(run_bash_test TEST_NAME SCRIPT_NAME)
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_NAME}.sh $<TARGET_FILE:membase> ${CMAKE_CURRENT_SOURCE_DIR}/config.json
    )
endfunction()

run_bash_test(BasicTest basic)
run_bash_test(ErrorsTets errors)
run_bash_test(FuzzingTest fuzzing)
run_bash_test(FindTest find)

add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS membase
)

