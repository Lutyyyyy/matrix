set (
    APPS
    main
    gen_test
)

foreach(APP ${APPS})
    add_subdirectory(${APP})
endforeach()

set(APPS "${APPS}" PARENT_SCOPE) 