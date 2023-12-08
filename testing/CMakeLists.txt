cmake_minimum_required(VERSION 3.20)

set(TARGET "test_grahpic_calculator_backend")

set(HEADERS
    ${LIB_DIR}/Tracer_lib/tracer.h
    test_backend/backend.h
    test_backend/servant.h
    test_backend/function_string.h
)
set(SOURCES
    ${LIB_DIR}/Tracer_lib/tracer.cpp
    main.cpp
    test_backend/function_string.cpp
    test_backend/segment_methods.cpp
)

project(${TARGET} CXX)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

install(TARGETS ${TARGET})