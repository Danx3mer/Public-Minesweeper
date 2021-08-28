#include "Engine.h"
#include "Label.h"
#include "Miner.h"

cell Cell[10][10];

std::fstream File;

extern Engine _Engine;

extern const int Pix_Cell_Size;

extern int Initial_Mine_Count;

extern Label B_NewGame;
extern Label B_EasyMode;
extern Label B_MediumMode;
extern Label B_HardMode;
extern Label B_RecordTime;
extern Label B_Time;
extern Label B_Mines_Left;
extern Label Sound_Button;

extern int ActionCounter;

extern HWND hwnd;

/// <summary>
/// Default constructor for Engine class.
/// </summary>
/// <param name="MineCount: ">Amount of mines to put on the field.</param>
Engine::Engine(int MineCount) :Mines_Left(MineCount), Game_End(false), Time_Start(false), Cells_used(0)
{
}
/// <summary>
/// (Re)Initializing new game.
/// </summary>
/// <param name="RestartGame: ">Wether or not the game is to be restarted.</param>
/// <param name="difficulty: ">Game type.</param>
void Engine::StartGame(bool RestartGame, int difficulty)
{
	static int diff = Mode_Easy;
	static int ActionCount{};
		if (difficulty != diff && difficulty != 0)
			diff = difficulty;
	if (RestartGame)
	{
		if (difficulty == Mode_Easy)
		{
			Initial_Mine_Count = 10;
			B_RecordTime.Min = _Engine.RTime.Easy.Min;
			B_RecordTime.Sec = _Engine.RTime.Easy.Sec;
		}
		else if (difficulty == Mode_Medium)
		{
			Initial_Mine_Count = 15;
			B_RecordTime.Min = _Engine.RTime.Medium.Min;
			B_RecordTime.Sec = _Engine.RTime.Medium.Sec;
		}
		else if (difficulty == Mode_Hard)
		{
			Initial_Mine_Count = 30;
			B_RecordTime.Min = _Engine.RTime.Hard.Min;
			B_RecordTime.Sec = _Engine.RTime.Hard.Sec;
		}

		Mines_Left = Initial_Mine_Count;

		B_Time.Min = 0;
		B_Time.Sec = 0;

		_Engine.Clear();

		Game_End = false;
	}
	//Drawing cells
	for (int row{}; row < 10; row++)
	{
		for (int column{}; column < 10; column++)
		{
			Cell[column][row].X = column + 1.0f;
			Cell[column][row].Y = row + 1.0f;
			Cell[column][row].Draw();
		}
	}

	DrawLabels(diff);
	CreateMines(RestartGame);
	if(RestartGame)	Time_Start = false;
	if (ActionCounter == 2 && _Engine.Game_End)
		ActionCounter++;
	else if (ActionCounter == 1 && _Engine.Game_End)
		ActionCounter++;
}
/// <summary>
/// Mine creation.
/// </summary>
/// <param name="Full_Reset: ">Resets all mines when true.</param>
void Engine::CreateMines(bool Full_Reset)
{
	static int ActionCounter{};
	if (Full_Reset) ActionCounter = 0;
	if (ActionCounter == 0)
	{
		srand(time(0));
		for (int i{}; i < Mines_Left; i++)
		{
			int RandX{ rand() % 10 };
			int RandY{ rand() % 10 };
			if (Cell[RandX][RandY].mine == false)
			{
				Cell[RandX][RandY].mine = true;
				SetNumber(RandX, RandY);
			}
			else
			{
				i--;
			}
		}
			ActionCounter++;
	}
}

/// <summary>
/// Set all game CELLS closed and default.
/// </summary>
void Engine::Clear()
{
	for (int row{}; row < 10; row++)
	{
		for (int column{}; column < 10; column++)
		{
			Cell[row][column].mine = false;
			Cell[row][column].open = false;
			Cell[row][column].flag_checked = false;
			Cell[row][column].number = 0;
		}
	}
	Cells_used = 0;
}

/// <summary>
/// Initial number setting on new game.
/// </summary>
/// <param name="x: ">Mine X coordinate.</param>
/// <param name="y: ">Mine Y coordinate.</param>
void Engine::SetNumber(int x, int y)
{
	for (int column = -1; column <= 1; column++)
	{
		for (int row = -1; row <= 1; row++)
		{
			if (_Engine.CellInMap((x + column + 1) * Pix_Cell_Size, (y + row + 3) * Pix_Cell_Size))
				Cell[x + column][y + row].number++;
		}
	}
}

/// <summary>
/// Called when Game is over.
/// </summary>
void Engine::GameOver()
{
	Time_Start = false;
	if (_Engine.Sound_On) PlaySound(L"Sounds/Arcade Explo A.wav", NULL, SND_ASYNC | SND_NOSTOP);
	MessageBoxCentered(hwnd, L"YOU STEPPED ON A MINE! 💥", L"GAMEOVER", MB_OK | MB_TASKMODAL, Window_W, Window_H);
	_Engine.Game_End = true;
	for (int r{}; r < 10; r++)
	{
		for (int c{}; c < 10; c++)
		{
			Cell[c][r].open = true;
		}
	}
}

/// <summary>
/// Checks if the user won.
/// </summary>
/// <returns>True when user won, otherwise false.</returns>
bool Engine::WinCheck()
{
	bool Win;
	Win = (Cells_used + _Engine.Mines_Left) == 100 ? true : false;
	if (Win == true)
	{
		for (int r{}; r < 10; r++)
		{
			for (int c{}; c < 10; c++)
			{
				if (Cell[c][r].mine == true)
					Cell[c][r].flag_checked = true;
			}
		}
	}
	return Win;
}

/// <summary>
/// Returns TRUE if clicked on game field.
/// </summary>
/// <param name="x: ">Coordinate X of mouse click position in pixels.</param>
/// <param name="y: ">Coordinate Y of mouse click position in pixels.</param>
bool Engine::CellInMap(int x, int y)
{
	if (x / Pix_Cell_Size >= 1 && x / Pix_Cell_Size <= 10)
	{
		if (y / Pix_Cell_Size >= 3 && y / Pix_Cell_Size <= 12)
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// Writes a new record to the file if the game was done in record time.
/// </summary>
void Engine::SetNewRecord()
{
	if (((B_Time.Min * 60) + B_Time.Sec) < (B_RecordTime.Sec + (B_RecordTime.Min * 60)) || (B_RecordTime.Min == 0 && B_RecordTime.Sec == 0))
	{
		MessageBoxCentered(hwnd, L"YOU WON!!!\n NEW RECORD!", L"CONGRATS!", MB_OK | MB_TASKMODAL, Window_W, Window_H);
		File.clear();
		File.seekg(0);
		B_RecordTime.Min = B_Time.Min;
		B_RecordTime.Sec = B_Time.Sec;

		if (Initial_Mine_Count == 10)
		{
			RTime.Easy.Min = B_Time.Min;
			RTime.Easy.Sec = B_Time.Sec;
		}
		if (Initial_Mine_Count == 15)
		{
			RTime.Medium.Min = B_Time.Min;
			RTime.Medium.Sec = B_Time.Sec;
		}
		if (Initial_Mine_Count == 30)
		{
			RTime.Hard.Min = B_Time.Min;
			RTime.Hard.Sec = B_Time.Sec;
		}
		LoadFile(Load_Out);
		return;
	}
	MessageBoxCentered(hwnd, L"YOU HAVE BEATEN MINESWEEPER!", L"WINNER!", MB_OK | MB_TASKMODAL, Window_W, Window_H);
}

/// <summary>
/// Opens empty cells if user clicked on empty cell.
/// </summary>
/// <param name="CX: ">X coordinate of click in opengl coordinates.</param>
/// <param name="CY: ">Y coordinate of click in opengl coordinates.</param>
/// <param name="PSC: ">Marks wether or not the sound should be a click sound.</param>
void Engine::Open_Empty_Cells(int CX, int CY, bool PSC)
{
	bool PlaySndClick{ PSC };
	if (!_Engine.CellInMap(CX * Pix_Cell_Size, (CY + 2) * Pix_Cell_Size) || Cell[CX - 1][CY - 1].open) return;
	if (Cell[CX - 1][CY - 1].mine)
	{
		Cell[CX - 1][CY - 1].open = true;
		PlaySndClick = false;
		return;
	}
	Cell[CX - 1][CY - 1].open = true;
	_Engine.Cells_used++;
	if (Cell[CX - 1][CY - 1].flag_checked) _Engine.Mines_Left++;
	if (Cell[CX - 1][CY - 1].number == 0)
	{
		if (_Engine.Sound_On) PlaySound(L"Sounds/Open Empty Cells.wav", NULL, SND_ASYNC | SND_NOSTOP);
		PlaySndClick = false;
		for (int column = -1; column <= 1; column++)
		{
			for (int row = -1; row <= 1; row++)
			{
				Open_Empty_Cells(CX + column, CY + row, false);
			}
		}
	}
	else
		if (PlaySndClick && _Engine.Sound_On)
			PlaySound(L"Sounds/Click.wav", NULL, SND_ASYNC | SND_NOSTOP);
}

/// <summary>
/// Loads the file into/out of the program.
/// </summary>
/// <param name="InOut: ">Marks wether to load the file into the program or out of the program.</param>
/// <returns>Wether or not the file is viable.</returns>
bool Engine::LoadFile(bool InOut)
{
	if (InOut == Load_In)
	{
		//Loading data Into Program
		File.open("Record.ini", std::ios::in | std::ios::binary);
		if (!File) { File.close(); return false; }
		File >> _Engine.RTime.Easy.Min;

		B_RecordTime.Min = _Engine.RTime.Easy.Min;

		File >> _Engine.RTime.Easy.Sec;

		B_RecordTime.Sec = _Engine.RTime.Easy.Sec;

		File >> _Engine.RTime.Medium.Min;
		File >> _Engine.RTime.Medium.Sec;

		File >> _Engine.RTime.Hard.Min;
		File >> _Engine.RTime.Hard.Sec;

		File >> _Engine.Sound_On;
		File.close();
	}
	else
	{
		//Loading data Out of Program
		File.open("Record.ini", std::ios::out | std::ios::binary);
		File << RTime.Easy.Min;
		File << ' ';
		File << RTime.Easy.Sec;
		File << ' ';
		File << RTime.Medium.Min;
		File << ' ';
		File << RTime.Medium.Sec;
		File << ' ';
		File << RTime.Hard.Min;
		File << ' ';
		File << RTime.Hard.Sec;
		File << ' ';
		File << _Engine.Sound_On;
		File.close();
	}
	return true;
}

#pragma region Drawing Functions

/// <summary>
/// Draws numbers on the main window.
/// </summary>
/// <param name="num: ">The number to be drawn.</param>
/// <param name="x: ">Position of the X coordinate.</param>
/// <param name="y: ">Position of the Y coordinate.</param>
/// <param name="color: ">Wether the number should be in color or not.</param>
/// <param name="negative: ">Wether the number is negative or not.</param>
void Engine::DrawNumber(int num, float x, float y, bool color, bool negative)
{
	glColor3f(0.0f, 0.0f, 0.0f); //setting black as default color if color==false
	if (negative) glColor3f(1.0f, 0.0f, 0.0f); //setting red as default color if negative==true

	switch (std::abs(num))
	{
	case 0:
	{
		if (color) glColor3f(0.5f, 0.93f, 0.93f);
		DrawTxt((char*)"0", x, y);
	}
	break;
	case 1:
	{
		if (color) glColor3f(0.0f, 0.93f, 0.0f);
		DrawTxt((char*)"1", x, y);
	}
	break;
	case 2:
	{
		if (color) glColor3f(0.0f, 0.0f, 1.0f);
		DrawTxt((char*)"2", x, y);
	}
	break;
	case 3:
	{
		if (color) glColor3f(1.0f, 0.0f, 0.0f);
		DrawTxt((char*)"3", x, y);
	}
	break;
	case 4:
	{
		if (color) glColor3f(0.0f, 0.5f, 0.5f);
		DrawTxt((char*)"4", x, y);
	}
	break;
	case 5:
	{
		if (color) glColor3f(0.5f, 0.0f, 1.0f);
		DrawTxt((char*)"5", x, y);
	}
	break;
	case 6:
	{
		if (color) glColor3f(0.0f, 0.5f, 1.0f);
		DrawTxt((char*)"6", x, y);
	}
	break;
	case 7:
	{
		if (color) glColor3f(0.7f, 0.5f, 0.5f);
		DrawTxt((char*)"7", x, y);
	}
	break;
	case 8:
	{
		if (color) glColor3f(0.5f, 0.23f, 1.0f);
		DrawTxt((char*)"8", x, y);
	}
	break;
	case 9:
	{
		if (color) glColor3f(0.23f, 1.0f, 1.0f);
		DrawTxt((char*)"9", x, y);
	}
	break;
	}
}

/// <summary>
/// Draws the labels on the screen.
/// </summary>
/// <param name="CurrentButton: ">The current mode button (Easy/Medium/Hard)</param>
void Engine::DrawLabels(int CurrentButton)
{
	B_NewGame.Draw(true, B_NewGame.X - 0.5f, B_NewGame.Y, B_NewGame.Width, B_NewGame.Height, false, true);
	if (CurrentButton == Mode_Easy)
	{
		B_EasyMode.Draw(true, B_EasyMode.X, B_EasyMode.Y, B_EasyMode.Width, B_EasyMode.Height, true);
		B_MediumMode.Draw(true, B_MediumMode.X, B_MediumMode.Y, B_MediumMode.Width, B_MediumMode.Height);
		B_HardMode.Draw(true, B_HardMode.X, B_HardMode.Y, B_HardMode.Width, B_HardMode.Height);
	}
	else if (CurrentButton == Mode_Medium)
	{
		B_EasyMode.Draw(true, B_EasyMode.X, B_EasyMode.Y, B_EasyMode.Width, B_EasyMode.Height);
		B_MediumMode.Draw(true, B_MediumMode.X, B_MediumMode.Y, B_MediumMode.Width, B_MediumMode.Height, true);
		B_HardMode.Draw(true, B_HardMode.X, B_HardMode.Y, B_HardMode.Width, B_HardMode.Height);
	}
	else if (CurrentButton == Mode_Hard)
	{
		B_EasyMode.Draw(true, B_EasyMode.X, B_EasyMode.Y, B_EasyMode.Width, B_EasyMode.Height);
		B_MediumMode.Draw(true, B_MediumMode.X, B_MediumMode.Y, B_MediumMode.Width, B_MediumMode.Height);
		B_HardMode.Draw(true, B_HardMode.X, B_HardMode.Y, B_HardMode.Width, B_HardMode.Height, true);
	}

	B_RecordTime.Draw(true, B_RecordTime.X + 0.3f, B_RecordTime.Y + 0.2f, B_RecordTime.Width, B_RecordTime.Height);
	B_Time.Draw(true, B_Time.X + 0.4f, B_Time.Y + 0.2f, B_Time.Width, B_Time.Height);
	B_Mines_Left.Draw(true, B_Mines_Left.X + 0.3f, B_Mines_Left.Y + 0.2f, B_Mines_Left.Width, B_Mines_Left.Height);
	B_Mines_Left.DrawMineCounter();
	Sound_Button.Draw(true, Sound_Button.X + 0.3f, Sound_Button.Y + 0.2f, Sound_Button.Width, Sound_Button.Height);
	Sound_Button.DrawSoundButton(_Engine.Sound_On);
}

/// <summary>
/// Draws text on the screen.
/// </summary>
/// <param name="str: ">The text to be drawn.</param>
/// <param name="x: ">X coordinates.</param>
/// <param name="y: ">Y coordinates.</param>
void Engine::DrawTxt(char* str, float x, float y)
{
	glRasterPos2f(x, y);
	for (char* c = str; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Updates the position
	}
}

#pragma endregion
