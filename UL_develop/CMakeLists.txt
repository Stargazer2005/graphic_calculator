cmake_minimum_required(VERSION 3.20)

set(TARGET "backend")

set(HEADERS
    ${LIB_DIR}/Tracer_lib/tracer.h
    back_ideas/backend.h
    back_ideas/helpers.h
)
set(SOURCES
    ${LIB_DIR}/Tracer_lib/tracer.cpp
    main.cpp
    back_ideas/graphic_string.cpp
)

project(${TARGET} CXX)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

install(TARGETS ${TARGET})