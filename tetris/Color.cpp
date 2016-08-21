#include "Color.h"

void createBrushes(HBRUSH brushes[]) 
{

	for (int i = 0; i < COLOR_AMOUNT; i++)
	{
		brushes[i] = CreateSolidBrush(colorValue[i]);
	}
}

void deleteBrushes(HBRUSH brushes[])
{
	for (int i = 0; i < COLOR_AMOUNT; i++)
	{
		DeleteObject(brushes[i]);
	}
}
