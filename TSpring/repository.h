#pragma once
#include<vector>
#include<afxstr.h>
#include"stdafx.h"
///List View
__declspec(selectany) std::vector<std::pair<CString, bool>>* g_image_data;
__declspec(selectany) std::vector<std::pair<CString, bool>>* g_class_data;
__declspec(selectany) bool* g_is_detection;
__declspec(selectany) bool* g_is_segmentation;
__declspec(selectany) bool* g_is_square;
__declspec(selectany) bool* g_is_rectangle;
__declspec(selectany) bool* g_is_rmid;
__declspec(selectany) bool* g_is_r2pt;
__declspec(selectany) bool* g_is_tracking;


__declspec(selectany) std::atomic<COLORREF> g_lv_color_bk = RGB(0, 43, 54);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_text = RGB(199, 199, 199);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_text_white = RGB(255, 255, 255);
__declspec(selectany) std::atomic<COLORREF> g_lv_color_border = RGB(211, 54, 130);

__declspec(selectany) int g_command_go2tagging = -1;

///Tag View
__declspec(selectany) int g_tag_idx = 0;
__declspec(selectany) std::atomic<COLORREF> g_tag_color_text_purple = RGB(108,113,196);
__declspec(selectany) std::atomic<COLORREF> g_tag_color_text_red = RGB(220,50,47);