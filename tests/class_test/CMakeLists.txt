set(
    TESTS
    Buffer_t_test
    Matrix_t_test
)

foreach(TEST ${TESTS})
    add_subdirectory(${TEST})
endforeach()

set(CLASS_TESTS "${TESTS}" PARENT_SCOPE)