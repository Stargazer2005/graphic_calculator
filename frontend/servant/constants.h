#pragma once

// std libs
#include <string>

namespace Front_consts {

const std::string empty_str = "";
// минимальное количество пикселей для ед. отрезка
constexpr int min_scale = 2;
// максимальное количество пикселей для ед. отрезка
constexpr int max_scale = 1024;
// максимальное количество вводимых функций пользователем (тип связан с использованием в векторах)
constexpr unsigned long long int max_functions_amount = 4;

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
}  // namespace Front_consts
