## Струкрутра проекта

```console
graphic_calculator/
|
|───frontend/
|       |───Graphix_calc/
|       |       |---Axis.cpp
|       |       |---Axis.h
|       |       |---Graphix.cpp
|       |       |---Graphix.h
|       |       |---Function_box.cpp
|       |       |---Function_box.h
|       |       |---Point_box.cpp
|       |       |---Point_box.h
|       |       |---Numbed_button.h
|       |       |---Segmented_Graphix.cpp
|       |       `---Segmented_Graphix.h
|       |
|       |───servant/
|       |       `---constants.h
|       |
|       |---Graphix_window.cpp
|       `---Graphix_window.h
|
|───backend/
|       |───Math_calc/ 
|       |       |---domain_segments.cpp
|       |       |---domain_segments.h
|       |       |---function_extremes.cpp
|       |       |---function_extremes.h
|       |       |---function_crosses.cpp
|       |       |---function_crosses.h
|       |       |---function_roots.cpp
|       |       |---function_roots.h
|       |       `---math_base.h
|       |
|       |───servant/
|       |       |---constants.h
|       |       |---servant.cpp
|       |       `---servant.h
|       |
|       |---expose_dep_function.cpp
|       |---expose_dep_function.h
|       |---function_string.cpp
|       |---function_string.h
|       `---function_string_checkers.cpp
|
|---frontend.h
|---backend.h
`---main.cpp

```

## Идеалогия

Фронтенд: пиксели и графики
Бэкенд: числа с плавающей запятой и функции

std::string func_strs - обозначающие математические функции
Backend::function funcs - екземпляры класса, с методом calculate, который используя func_strs считает y по x которые
Graphix_calc::Graphix graphics - фигуры, представляющие собой графики, построенные по funcs

(для производных der = func, deriative = graphic)
