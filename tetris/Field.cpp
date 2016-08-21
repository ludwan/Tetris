#include "Field.h"

brick field[FIELD_HEIGHT][FIELD_WIDTH + INFO_WIDTH];

void createField(int x, int y) 
{
	for (int row = 0; row < FIELD_HEIGHT; row++)
	{
		for (int col = 0; col < FIELD_WIDTH + INFO_WIDTH; col++)
		{
			if (col == 0 || col >= FIELD_WIDTH - 1 || row == FIELD_HEIGHT - 1)
				field[row][col].color = GRAY;
			else
				field[row][col].color = BLACK;

			SetRect(&field[row][col].rect, x + (BRICK_WIDTH * col) + 1, y + (BRICK_HEIGHT * row) + 1, x + (BRICK_WIDTH * col + BRICK_WIDTH) - 1, y + (BRICK_HEIGHT * row + BRICK_HEIGHT) - 1);
		}
	}

	for (int row = 2; row < 7; row++)
	{
		for (int col = 12; col < 17; col++)
		{
			field[row][col].color = BLACK;
		}
	}
}

void drawField(HDC hdc, HBRUSH brushes[])
{
	for (int i = 0; i < FIELD_HEIGHT - 1; i++)
	{
		for (int j = 1; j < FIELD_WIDTH - 1; j++)
		{
			FillRect(hdc, &field[i][j].rect, brushes[field[i][j].color]);
		}
	}

	for (int row = 2; row < 7; row++)
	{
		for (int col = 12; col < 17; col++)
		{
			FillRect(hdc, &field[row][col].rect, brushes[field[row][col].color]);
		}
	}
}

void fillField(int color)
{
	for (int i = 0; i < FIELD_HEIGHT - 1; i++)
	{
		for (int j = 1; j < FIELD_WIDTH - 1; j++)
		{
			field[i][j].color = color;
		}
	}

	for (int row = 2; row < 7; row++)
	{
		for (int col = 12; col < 17; col++)
		{
			field[row][col].color = color;
		}
	}
}

void removeFullRow(int y)
{
	for (int i = y; i > 0; i--)
	{
		for (int j = 1; j < FIELD_WIDTH - 1; j++)
		{
			field[i][j].color = field[i - 1][j].color;
		}
	}
}

int checkFullRow(int y)
{
	int count = 0;
	for (int i = 1; i < FIELD_WIDTH - 1; i++)
	{
		if (field[y][i].color != BLACK)
			count++;
	}

	if (count == FIELD_WIDTH - 2)
		return y;

	return -1;
}