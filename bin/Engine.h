#pragma once

#include "MessageBox.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "cell.h"
#include "time.h"

struct RecordTime
{
	struct TimeMS
	{
		int Min;
		int Sec;
	};
	TimeMS Easy;
	TimeMS Medium;
	TimeMS Hard;
};

extern cell Cell[10][10];
class Engine
{
public:
	Engine(int);

	void Open_Empty_Cells(int CX, int CY, bool PSC = true);

	void StartGame(bool RestartGame, int difficulty);
	void CreateMines(bool);
	void SetNumber(int x, int y);

	void Clear();
	void GameOver();
	void SetNewRecord();
	bool LoadFile(bool InOut);

	bool WinCheck();
	bool CellInMap(int x, int y);

	void DrawLabels(int CurrentButton);
	void DrawNumber(int num, float x, float y, bool color = true, bool negative = false);
	void DrawTxt(char* str, float x, float y);

public:
	int Mines_Left;
	int Cells_used;

	bool Time_Start;
	bool Game_End;
	bool Sound_On;

	RecordTime RTime;
};
