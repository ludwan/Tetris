#include "Sound.h"

void loadMusic()
{
	mciSendString("open \"tetris.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
}

void playMusic()
{
	mciSendString("play mp3 repeat", NULL, 0, NULL);
}

void closeMusic()
{
	mciSendString("close mp3", NULL, 0, NULL);
}

void pauseMusic()
{
	mciSendString("pause mp3", NULL, 0, NULL);
}

void resumeMusic()
{
	mciSendString("resume mp3", NULL, 0, NULL);
}