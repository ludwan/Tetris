#include "Piece.h"
#include <Windows.h>

void createPiece(piece *currPiece, piece *nextPiece)
{
	currPiece->rotation = nextPiece->rotation;
	currPiece->shape = nextPiece->shape;
	currPiece->color = nextPiece->color;
	currPiece->x = 4;
	currPiece->y = 0;

	nextPiece->shape = rand() % 7;
	nextPiece->rotation = rand() % 4;
	nextPiece->color = nextPiece->shape;
	nextPiece->x = 13;
	nextPiece->y = 3;
}

void deletePiece(piece *piece)
{
	delete piece;
}

BOOL checkPiece(piece *piece)
{
	int col = 0, row = 0;

	for (int bit = 0x8000; bit >= 0x0001; bit >>= 1)
	{
		if (pieces[piece->shape][piece->rotation] & bit)
		{
			if (field[piece->y + row][piece->x + col].color != BLACK)
			{
				return FALSE;
			}
		}
		col++;
		if (col == 4)
		{
			row++;
			col = 0;
		}
	}

	return TRUE;
}

void erasePiece(piece *piece)
{
	int col = 0, row = 0;

	for (int bit = 0x8000; bit >= 0x0001; bit >>= 1)
	{
		if (pieces[piece->shape][piece->rotation] & bit)
		{
			field[piece->y + row][piece->x + col].color = BLACK;
		}
		col++;
		if (col == 4)
		{
			row++;
			col = 0;
		}
	}

}

void drawPiece(piece *piece)
{
	int col = 0, row = 0;

	for (int bit = 0x8000; bit >= 0x0001; bit >>= 1)
	{
		if (pieces[piece->shape][piece->rotation] & bit)
		{
			field[piece->y + row][piece->x + col].color = piece->color;
		}
		col++;
		if (col == 4)
		{
			row++;
			col = 0;
		}
	}
}

void rotatePiece(piece *piece)
{
	int prevRot = piece->rotation;
	piece->rotation++;
	if (piece->rotation == 4)
		piece->rotation = 0;

	if (!checkPiece(piece))
		piece->rotation = prevRot;
}

BOOL movePieceDown(piece *piece)
{

	++piece->y;
	if (!checkPiece(piece))
	{
		--piece->y;
		return FALSE;
	}
	return TRUE;
}

void movePieceRight(piece *piece)
{
	piece->x++;

	if (!checkPiece(piece))
		piece->x--;
}

void movePieceLeft(piece *piece)
{
	--piece->x;

	if (!checkPiece(piece))
		++piece->x;
}

int dropPiece(piece *piece)
{
	int startHeight = piece->y;

	while (movePieceDown(piece));

	return piece->y - startHeight;
}