cmake_minimum_required(VERSION 3.20)

set(TARGET "backend")

set(HEADERS
    functions.h
)
set(SOURCES
    main.cpp
    functions.cpp
)

project(${TARGET} CXX)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

install(TARGETS ${TARGET})