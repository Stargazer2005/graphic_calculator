cmake_minimum_required(VERSION 3.20)

set(TARGET "graphic_calculator")

set(HEADERS
# Graph_lib
    frontend/Graph_lib/Graph.h
    frontend/Graph_lib/GUI.h
    frontend/Graph_lib/Point.h
    frontend/Graph_lib/Window.h
    frontend/Graph_lib/fltk.h

# front
    frontend/Graphix_calc/Axis.h
    frontend/Graphix_calc/Graphix.h
    frontend/Graphix_calc/Function_box.h
    frontend/Graphix_calc/Point_box.h
    frontend/Graphix_calc/Bothput_box.h
    frontend/Graphix_calc/Numbed_button.h
    frontend/Graphix_calc/Segmented_Graphix.h

    frontend/utility/utilities.h
    frontend/utility/constants.h
    
    frontend/graphix_window/Graphix_window.h
    
    frontend.h

# back
    backend/Math_calc/domain_segments.h
    backend/Math_calc/function_extremes.h
    backend/Math_calc/function_crosses.h
    backend/Math_calc/function_roots.h
    backend/Math_calc/math_base.h

    backend/utility/utilities.h
    backend/utility/constants.h

    backend/Math_func/expose_func_str.h
    backend/Math_func/function.h

    backend.h
)

set(SOURCES
# Graph_lib
    frontend/Graph_lib/Graph.cpp
    frontend/Graph_lib/GUI.cpp
    frontend/Graph_lib/Window.cpp

# front
    frontend/Graphix_calc/Axis.cpp
    frontend/Graphix_calc/Graphix.cpp
    frontend/Graphix_calc/Function_box.cpp
    frontend/Graphix_calc/Point_box.cpp
    frontend/Graphix_calc/Bothput_box.cpp
    frontend/Graphix_calc/Segmented_Graphix.cpp

    frontend/utility/utilities.cpp

    frontend/graphix_window/Graphix_window.cpp
    frontend/graphix_window/Graphix_window_updaters.cpp
    
# back
    backend/Math_calc/domain_segments.cpp
    backend/Math_calc/function_extremes.cpp
    backend/Math_calc/function_crosses.cpp
    backend/Math_calc/function_roots.cpp

    backend/utility/utilities.cpp

    backend/Math_func/expose_func_str.cpp
    backend/Math_func/function.cpp
    backend/Math_func/function_validation.cpp

# main
    main.cpp
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
