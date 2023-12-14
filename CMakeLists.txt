cmake_minimum_required(VERSION 3.20)

set(TARGET "graphic_calculator")

set(HEADERS
#front
    frontend/graphix_window.h

    frontend/Graphix_calc/Axis.h
    frontend/Graphix_calc/Segmented_function.h
    frontend/Graphix_calc/Input_box.h
    frontend/Graphix_calc/Numbed_button.h

    frontend/servant/constants.h
    
    frontend.h

#back
    backend/function_string.h

    backend/Math_calc/math_base.h
    backend/Math_calc/function_roots.h
    backend/Math_calc/function_crosses.h
    backend/Math_calc/domain_segments.h

    backend/servant/servant.h
    backend/servant/constants.h

    backend.h

    ${LIB_DIR}/Graph_lib/Graph.h
    ${LIB_DIR}/Graph_lib/GUI.h
    ${LIB_DIR}/Graph_lib/Point.h
    ${LIB_DIR}/Graph_lib/Window.h
)

set(SOURCES
#front
    frontend/graphix_window.cpp

    frontend/Graphix_calc/Axis.cpp
    frontend/Graphix_calc/Input_box.cpp
    frontend/Graphix_calc/Segmented_function.cpp
    
#back
    backend/function_string.cpp
    backend/servant/servant.cpp
    backend/function_string_checkers.cpp
    backend/Math_calc/domain_segments.cpp
    backend/Math_calc/function_roots.cpp
    backend/Math_calc/function_crosses.cpp
    
#main
    main.cpp

    ${LIB_DIR}/Graph_lib/Graph.cpp
    ${LIB_DIR}/Graph_lib/GUI.cpp
    ${LIB_DIR}/Graph_lib/Window.cpp
)

project(${TARGET} CXX)

set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FORMS True)

find_package(FLTK 1.3.8 EXACT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${FLTK_INCLUDE_DIR})
link_directories(${FLTK_INCLUDE_DIR}/../lib)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${TARGET} fltk_jpeg fltk_png fltk_z)
endif()

install(TARGETS ${TARGET})
