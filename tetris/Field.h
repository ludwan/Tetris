#pragma once
#include "Color.h"

const int INFO_WIDTH = 6;

const int FIELD_WIDTH = 12;
const int FIELD_HEIGHT = 28;

const int BRICK_WIDTH = 20;
const int BRICK_HEIGHT = 20;

struct brick
{
	RECT rect;
	int color;
};

extern brick field[FIELD_HEIGHT][FIELD_WIDTH + INFO_WIDTH];

void createField(int x, int y);
void drawField(HDC hdc, HBRUSH brushes[]);
void fillField(int color);
void removeFullRow(int y);
int checkFullRow(int y);