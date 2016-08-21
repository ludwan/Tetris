#include <time.h>
#include <sstream>
#include "Piece.h"
#include "Sound.h"

const int ID_NEW = 1;
const int ID_QUIT = 2;
const int ID_ABOUT = 3;

const int startSpeed = 300;
const int minSpeed = 90;

using namespace std;
const string APPTITLE = "Tetris";

SHORT lastKeyPressed = 0;
DWORD lastKeyPressedTime = 0;

int lines = 0, score = 0, level = 0;

HDC device = NULL;

HWND window;
HDC hdcBuffer = NULL, hdcBackground = NULL;
HBITMAP hbmBuffer = NULL, hbmBackground = NULL;

DWORD current_time = 0, last_time = 0;

HBRUSH brushes[COLOR_AMOUNT] = { 0 };

BOOL pieceDown = FALSE;
BOOL gameRunning = FALSE;
BOOL gameStarted = FALSE;
BOOL MusicPlaying = FALSE;

piece *currPiece; 
piece *nextPiece;

int speed = 0;

void drawWindow();

void setSpeed(int *speed, int level)
{
	*speed = startSpeed - 5 * level;

	if (*speed < minSpeed)
	{
		*speed = minSpeed;
	}
}

void newGame()
{
	gameStarted = TRUE;
	gameRunning = TRUE;

	fillField(BLACK);
	level = 0;
	score = 0;
	lines = 0;

	setSpeed(&speed, level);

	createPiece(currPiece, nextPiece);
	createPiece(currPiece, nextPiece);

	drawPiece(currPiece);
	drawPiece(nextPiece);
}

void addScore(int rows, int *score, int level)
{
	switch (rows)
	{
	case 0:
		break;

	case 1:
		*score += (40 * (level + 1));
		break;

	case 2:
		*score += (100 * (level + 1));
		break;

	case 3:
		*score += (300 * (level + 1));
		break;

	case 4:
		*score += (1200 * (level + 1));
		break;

	}
}



void processInput() 
{

	if(lastKeyPressed)
	{
		if (GetAsyncKeyState(lastKeyPressed) & 0x8000)
		{
			if ((timeGetTime() - lastKeyPressedTime >= 200))
				lastKeyPressed = 0;

			else
				return;
		}
		else
			lastKeyPressed = 0;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000) && gameRunning)
	{
		erasePiece(currPiece);
		rotatePiece(currPiece);
		drawPiece(currPiece);

		lastKeyPressed = VK_UP;
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && gameRunning)
	{
		erasePiece(currPiece);
		movePieceRight(currPiece);
		drawPiece(currPiece);

		lastKeyPressed = VK_RIGHT;
	}
	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && gameRunning)
	{
		erasePiece(currPiece);
		movePieceLeft(currPiece);
		drawPiece(currPiece);

		lastKeyPressed = VK_LEFT;
	}
	else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && gameRunning)
	{
		erasePiece(currPiece);
		score += dropPiece(currPiece);
		drawPiece(currPiece);

		lastKeyPressed = VK_DOWN;
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !gameStarted && !gameRunning)
	{
		newGame();

		lastKeyPressed = VK_SPACE;
	}
	else if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && gameStarted)
	{
		gameRunning = !gameRunning;

		lastKeyPressed = VK_SPACE;
	}
	else if (GetAsyncKeyState(0x50) & 0x8000)
	{
		MusicPlaying = !MusicPlaying;
		if (MusicPlaying == TRUE)
		{
			resumeMusic();
		}
		else
		{
			pauseMusic();
		}
		lastKeyPressed = 0x50;
	}

	if (lastKeyPressed)
		lastKeyPressedTime = timeGetTime();
}

void drawWindow()
{
	ostringstream oss;
	device = GetDC(window);


	BitBlt(hdcBuffer, 0, 0, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT), hdcBackground, 0, 0, SRCCOPY);

	drawField(hdcBuffer, brushes);

	
	oss << level;
	string levelString = oss.str();
	TextOut(hdcBuffer, BRICK_WIDTH * (FIELD_WIDTH - 1) + BRICK_WIDTH / 2, BRICK_HEIGHT * 9 + BRICK_HEIGHT/2, levelString.c_str(), strlen(levelString.c_str()));
	oss.str("");
	oss.clear();

	oss << lines;
	string linesString = oss.str();
	TextOut(hdcBuffer, BRICK_WIDTH * (FIELD_WIDTH - 1) + BRICK_WIDTH / 2, BRICK_HEIGHT * 13 + BRICK_HEIGHT / 2, linesString.c_str(), strlen(linesString.c_str()));
	oss.str("");
	oss.clear();

	oss << score;
	string scoreString = oss.str();
	TextOut(hdcBuffer, BRICK_WIDTH * (FIELD_WIDTH - 1) + BRICK_WIDTH/2, BRICK_HEIGHT * 17 + BRICK_HEIGHT / 2, scoreString.c_str(), strlen(scoreString.c_str()));

	BitBlt(device, 0, 0, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT), hdcBuffer, 0, 0, SRCCOPY);
	ReleaseDC(window, device);
}

void blinkRows(int y[])
{
	int counter = 0;
	DWORD currT = 0, lastT = 0;
	while (counter <= 6)
	{
		currT = timeGetTime();
		if (currT - lastT > 75)
		{
			lastT = currT;
			for (int i = 0; i < 4; i++)
			{
				if (y[i] != 0)
				{
					for (int j = 1; j < FIELD_WIDTH - 1; j++)
					{
						field[y[i]][j].color = (counter % 2) + 8;
						drawWindow();
					}
				}
			}
			counter++;
		}
	}
}

int removeRows(piece *piece)
{
	int fullRows = 0;
	int rows[4] = { 0 };
	int currRow;
	for (int i = piece->y; i < FIELD_HEIGHT - 1; i++)
	{
		currRow = checkFullRow(i);
		if (currRow != -1)
		{
			rows[fullRows] = currRow;
			fullRows++;
		}
	}

	blinkRows(rows);
	for (int i = 0; i < 4; i++)
	{
		if (rows[i] != 0)
			removeFullRow(rows[i]);
	}

	return fullRows;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CLOSE:
		{
			if (MessageBox(hWnd, "Are you sure you want to exit?", APPTITLE.c_str(), MB_YESNO) == IDYES)
				PostQuitMessage(0);

			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps = { 0 };
			HDC hdc = BeginPaint(window, &ps);
			EndPaint(window, &ps);
			return 0;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_QUIT:
			if (MessageBox(hWnd, "Are you sure you want to exit?", APPTITLE.c_str(), MB_YESNO) == IDYES)
				PostQuitMessage(0);

			return 0;

		case ID_ABOUT:
			MessageBox(window, "About this program!", "About", MB_OK);
			break;

		case ID_NEW:
			newGame();
			break;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill the struct with info
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.hIconSm = NULL;

	//set up the window with the class info
	return RegisterClassEx(&wc);
}

void setupBackground()
{
	HDC hdc = GetDC(window);

	hdcBuffer = CreateCompatibleDC(hdc);
	hbmBuffer = CreateCompatibleBitmap(hdc, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT));
	SelectObject(hdcBuffer, hbmBuffer);
	SetTextColor(hdcBuffer, RGB(255, 0, 255));
	SetBkMode(hdcBuffer, TRANSPARENT);

	hdcBackground = CreateCompatibleDC(hdc);
	hbmBackground = CreateCompatibleBitmap(hdc, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT));
	SelectObject(hdcBackground, hbmBackground);

	ReleaseDC(window, hdc);

	createField(-BRICK_WIDTH, 0);
	

	RECT r;
	SetRect(&r, 0, 0, BRICK_WIDTH * (FIELD_WIDTH - 2), BRICK_HEIGHT * (FIELD_HEIGHT));
	FillRect(hdcBackground, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));

	SetRect(&r, BRICK_WIDTH * (FIELD_WIDTH - 2), 0, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT));
	FillRect(hdcBackground, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

	DrawEdge(hdcBackground, &r, EDGE_BUMP, BF_RECT);

	RECT n;
	SetRect(&n, BRICK_WIDTH * (FIELD_WIDTH - 1) - 1, BRICK_HEIGHT * 2 - 1, BRICK_WIDTH * (FIELD_WIDTH + 4) + 1, BRICK_HEIGHT * 7 + 1);
	FillRect(hdcBackground, &n, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawEdge(hdcBackground, &n, EDGE_BUMP, BF_RECT);

	SetTextColor(hdcBackground, RGB(255, 0, 255));
	SetBkMode(hdcBackground, TRANSPARENT);

	TextOut(hdcBackground, n.left, n.top - BRICK_HEIGHT, "Next", strlen("Next"));

	RECT l;
	SetRect(&l, BRICK_WIDTH * (FIELD_WIDTH - 1), BRICK_HEIGHT * 9, BRICK_WIDTH * (FIELD_WIDTH + 4), BRICK_HEIGHT * 11);
	FillRect(hdcBackground, &l, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawEdge(hdcBackground, &l, EDGE_BUMP, BF_RECT);
	TextOut(hdcBackground, l.left, l.top - BRICK_HEIGHT, "Level", strlen("Level"));

	RECT li;
	SetRect(&li, BRICK_WIDTH * (FIELD_WIDTH - 1), BRICK_HEIGHT * 13, BRICK_WIDTH * (FIELD_WIDTH + 4), BRICK_HEIGHT * 15);
	FillRect(hdcBackground, &li, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawEdge(hdcBackground, &li, EDGE_BUMP, BF_RECT);
	TextOut(hdcBackground, li.left, li.top - BRICK_HEIGHT, "Lines", strlen("Lines"));

	RECT s;
	SetRect(&s, BRICK_WIDTH * (FIELD_WIDTH - 1), BRICK_HEIGHT * 17, BRICK_WIDTH * (FIELD_WIDTH + 4), BRICK_HEIGHT * 19);
	FillRect(hdcBackground, &s, (HBRUSH)GetStockObject(BLACK_BRUSH));
	DrawEdge(hdcBackground, &s, EDGE_BUMP, BF_RECT);
	TextOut(hdcBackground, s.left, s.top - BRICK_HEIGHT, "Score", strlen("Score"));

	TextOut(hdcBackground, BRICK_WIDTH * (FIELD_WIDTH - 2) + BRICK_WIDTH / 2, BRICK_HEIGHT * 21, "Play/Pause Game", strlen("Play/Pause Game"));
	TextOut(hdcBackground, BRICK_WIDTH * (FIELD_WIDTH - 1) + BRICK_WIDTH / 2, BRICK_HEIGHT * 22, " - Space", strlen(" - Space"));

	TextOut(hdcBackground, BRICK_WIDTH * (FIELD_WIDTH - 2) + BRICK_WIDTH / 2, BRICK_HEIGHT * 24, "Play/Pause Audio", strlen("Play/Pause Audio"));
	TextOut(hdcBackground, BRICK_WIDTH * (FIELD_WIDTH) + BRICK_WIDTH / 2, BRICK_HEIGHT * 25, " - P", strlen(" - P"));
}

void deleteGraphics()
{
	DeleteDC(hdcBackground);
	DeleteObject(hbmBackground);

	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void updateGame()
{
	current_time = timeGetTime();

	if (gameRunning)
	{
		if (current_time - last_time >= speed)
		{
			last_time = current_time;
			erasePiece(currPiece);
			pieceDown = movePieceDown(currPiece);
			drawPiece(currPiece);

			if (!pieceDown)
			{
				int fullRows = removeRows(currPiece);
				lines += fullRows;
				level = lines / 10;
				setSpeed(&speed, level);
				addScore(fullRows, &score, level);
				erasePiece(nextPiece);
				createPiece(currPiece, nextPiece);

				if (!checkPiece(currPiece))
				{
					drawPiece(currPiece);
					gameRunning = false;
					gameStarted = false;
				}
				else
				{
					drawPiece(currPiece);
					drawPiece(nextPiece);
				}
			}
		}
	}
}

HMENU CreateMainMenu()
{
	HMENU menu = CreateMenu();

	HMENU file = CreateMenu();
	AppendMenu(file, MF_STRING, ID_NEW, "&New - Space");
	AppendMenu(file, MF_SEPARATOR, 0, 0);
	AppendMenu(file, MF_STRING, ID_QUIT, "&Quit");

	AppendMenu(menu, MF_POPUP, (UINT_PTR)file, "&File");

	HMENU help = CreateMenu();
	AppendMenu(help, MF_STRING, ID_ABOUT, "&About");

	AppendMenu(menu, MF_POPUP, (UINT_PTR)help, "&Help");

	return menu;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//create a new window
	DWORD dwStyle = WS_POPUP | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
	DWORD dwExStyle = 0;
	RECT r, w;
	SetRect(&r, 0, 0, BRICK_WIDTH * (FIELD_WIDTH + INFO_WIDTH - 1), BRICK_HEIGHT * (FIELD_HEIGHT));
	AdjustWindowRectEx(&r, dwStyle, FALSE, dwExStyle);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &w, 0);
	
	int width = r.right - r.left;
	int height = r.bottom - r.top;
	int x = ((w.right - w.left) / 2) - (width / 2);
	int y = ((w.bottom - w.top) / 2) - (height / 2);
	window = CreateWindowEx(
		dwExStyle,
		APPTITLE.c_str(),              //window class
		APPTITLE.c_str(),              //title bar
		dwStyle,   //window style
		x,         //x position of window
		y,         //y position of window
		width,                   //width of the window
		height,                   //height of the window
		NULL,                  //parent window
		CreateMainMenu(),                  //menu
		hInstance,             //application instance
		NULL);                 //window parameters

							   //was there an error creating the window?
	if (window == 0) return 0;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;
	createBrushes(brushes);
	srand(time(NULL));
	loadMusic();
	playMusic();

	currPiece = new piece();
	nextPiece = new piece();

	setupBackground();

	drawWindow();
	MSG msg = { 0 };
	last_time = timeGetTime();
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			processInput();
			updateGame();
		}
		drawWindow();
	}
	closeMusic();
	deletePiece(currPiece);
	deletePiece(nextPiece);
	deleteBrushes(brushes);
	deleteGraphics();
	return 0;
}
