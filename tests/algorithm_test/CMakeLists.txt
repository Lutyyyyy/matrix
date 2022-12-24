set(
    TESTS
    determinant_computing_algorithm_test
)

foreach(TEST ${TESTS})
    add_subdirectory(${TEST})    
endforeach()

set (TESTS "${TESTS}" PARENT_SCOPE)