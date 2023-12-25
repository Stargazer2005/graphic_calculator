#pragma once

// std libs
#include <string>

// Graphix_calc
#include "../Graphix_calc/Graphix.h"
#include "../Graphix_calc/Segmented_Graphix.h"

// Backend
#include "../../backend.h"

namespace Frontend_consts {

const std::string empty_str = "";

// (мы можем использовать такую запись функции не боясь за точки пересечения)
// (это связано с тем, что у нас в любом случае изначально enter_menu[i]->is_input_valid = false)
// (т.е. у только что созданных новых Function_box'ов не берется y = 0 в расчет для точек)
const Math_func::function empty_func{"0"};

// минимальное количество пикселей для ед. отрезка
constexpr int min_scale = 2;
// максимальное количество пикселей для ед. отрезка
constexpr int max_scale = 1024;
// максимальное количество вводимых функций пользователем (тип связан с использованием в векторах)
constexpr unsigned long long int max_functions_amount = 6;

constexpr double scale_coef = 2;

constexpr int margin = 20;
constexpr int padding = 10;

// стандартная ширина кнопки
constexpr int btn_w = 70;
// стандартная высота кнопки
constexpr int btn_h = 20;

// сторона квадратной кнопки (кнопки изменения масштаба)
constexpr int scl_btn_side = 30;

// пиксели, захваченные лейблом для in_box (т.е. "y = ")
constexpr int in_box_lab_w = 30;

// длина поля для ввода исключая его лейбл (т.е. "y = ")
constexpr int in_box_w = btn_w * 3 + padding * 2 - in_box_lab_w;
// высота поля для ввода
constexpr int in_box_h = 30;

// длина поля вывода
constexpr int out_box_w = in_box_w + in_box_lab_w;
// высота поля вывода
constexpr int out_box_h = 20;

// длина всей структуры, где пользователь может рисовать графики
constexpr int func_box_w = out_box_w;
// высота всей структуры, где пользователь может рисовать графики
constexpr int func_box_h = in_box_h + 2 * btn_h + out_box_h + padding;

constexpr int height_buff = 3;

constexpr int distance = 50;

}  // namespace Frontend_consts
