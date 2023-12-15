#pragma once

// std libs
#include <string>

namespace Front_consts {

const std::string empty_str = "";
constexpr int min_scale = 3;
constexpr int max_scale = 100000;
constexpr int graphs_max_amount = 4;
constexpr double scale_coef = 2;

constexpr int margin = 20;
constexpr int padding = 10;

constexpr int btn_w = 70;
constexpr int btn_h = 20;

constexpr int scl_btn_side = 30;

constexpr int inp_box_w = 200;
constexpr int inp_box_h = 30;
constexpr int inp_box_cap_w = 30;
constexpr int inp_box_all_w = inp_box_w + inp_box_cap_w;
constexpr int out_box_w = inp_box_all_w;
constexpr int out_box_h = 20;
constexpr int inp_box_indent_h = inp_box_h + btn_h + out_box_h + padding;

constexpr int height_buff = 3;

}  // namespace Front_consts
