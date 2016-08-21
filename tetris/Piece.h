#pragma once
#include "Field.h"

struct piece
{
	int x, y;
	int shape;
	int rotation;
	int color;
};

const int pieces[7][4] =
{
	{ 0x0C60, 0x4C80, 0xC600, 0x2640 },
	{ 0x4460, 0x0E80, 0xC440, 0x2E00 },
	{ 0xCC00, 0xCC00, 0xCC00, 0xCC00 },
	{ 0x06C0, 0x8C40, 0x6C00, 0x4620 },
	{ 0x44C0, 0x8E00, 0x6440, 0x0E20 },
	{ 0x0F00, 0x2222, 0x00F0, 0x4444 },
	{ 0x0E40, 0x4C40, 0x4E00, 0x4640 }
};

void createPiece(piece *currPiece, piece *nextPiece);
void deletePiece(piece *piece);
BOOL checkPiece(piece *piece);
void erasePiece(piece *piece);
void drawPiece(piece *piece);
void rotatePiece(piece *piece);
BOOL movePieceDown(piece *piece);
void movePieceRight(piece *piece);
void movePieceLeft(piece *piece);
int dropPiece(piece *piece);
