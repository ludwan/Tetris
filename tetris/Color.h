#pragma once
#include <Windows.h>
const int COLOR_AMOUNT = 10;

enum colorType { RED = 0, ORANGE, YELLOW, GREEN, BLUE, CYAN, PURPLE, BLACK, GRAY, WHITE };
const COLORREF colorValue[COLOR_AMOUNT] = { RGB(255, 0, 0), RGB(255, 128, 0), RGB(255, 255, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(0, 255, 255), RGB(128, 0, 128), RGB(0, 0, 0), RGB(128, 128, 128), RGB(0, 0, 0) };

void createBrushes(HBRUSH brushes[]);
void deleteBrushes(HBRUSH brushes[]);