## Струкрутра проекта

```console
graphic_calculator/
|
|───frontend/
|       |───Graphix_calc/
|       |       |---Axis.cpp
|       |       |---Axis.h
|       |       |---Function_box.cpp
|       |       |---Function_box.h
|       |       |---Point_box.cpp
|       |       |---Point_box.h
|       |       |---Numbed_button.h
|       |       |---Segmented_function.cpp
|       |       `---Segmented_function.h
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

## Гайд по ошибкам

ошибки с expression - ошибки при использовании y_n
ошибки с function - ошибки при использовании tan(x), sin(x) и т.п.

## Идеалогия

Фронтенд: пиксели и графики
Бэкенд: числа с плавающей запятой и функции
