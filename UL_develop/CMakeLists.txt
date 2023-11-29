cmake_minimum_required(VERSION 3.20)

set(TARGET "backend")

set(HEADERS
    back_func.h
)
set(SOURCES
    main.cpp
    back_func.cpp
)

project(${TARGET} CXX)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

install(TARGETS ${TARGET})