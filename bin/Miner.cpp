#include "Engine.h"
#include "Miner.h"
#include "Label.h"

extern const int Pix_Cell_Size = Window_W / 12;

Label B_NewGame((char*)"Start New Game", 0.9f, 12.2f, 10.0f, 0.6f, false);
Label B_EasyMode((char*)"    Easy", 0.9f, 11.6f, 3.3333f, 0.6f, false);
Label B_MediumMode((char*)" Meduim", 4.2333f, 11.6f, 3.3333f, 0.6f, false);
Label B_HardMode((char*)"    Hard", 7.5666f, 11.6f, 3.3333f, 0.6f, false);
Label B_RecordTime((char*)"Record:", 0.9f, -0.5f, 3.0f, 1.0f, true);
Label B_Time((char*)"Time:", 7.9f, -0.5f, 3.0f, 1.0f, true);
Label B_Mines_Left((char*)"Mines:", 4.9f, -0.5f, 2.0f, 1.0f, false);
Label Sound_Button((char*)"", 5.5f, 13.0f, 1.0f, 0.8f, false);

int Initial_Mine_Count{ Mode_Easy };
Engine _Engine(Initial_Mine_Count);
extern HWND hwnd;
extern int ActionCounter{};

/// <summary>
/// A callback function that displays everything onscreen.
/// </summary>
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 2, 0);

	_Engine.StartGame(false, 0);

	glutSwapBuffers();

	if (_Engine.WinCheck())
	{
		_Engine.Time_Start = false;
		_Engine.Game_End = true;

		if (ActionCounter == 2)
		{
			_Engine.Mines_Left = 0;
			_Engine.StartGame(false, 0);
			glutSwapBuffers();
			if (_Engine.Sound_On) PlaySound(L"Sounds/FF_CAPT.wav", NULL, SND_ASYNC);
			_Engine.SetNewRecord();
			ActionCounter += 2;
			return;
		}
	}
	if (ActionCounter == 3)
	{
		_Engine.GameOver();
		ActionCounter++;
		return;
	}
}

/// <summary>
/// A callback function that resizes the application window.
/// </summary>
/// <param name="W: ">New width of the window.</param>
/// <param name="H: ">New height of the window.</param>
void Reshape(int W, int H)
{
	glutReshapeWindow(Window_W, Window_H);
	glViewport(0, 0, (GLsizei)W, (GLsizei)H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 12, 0, 16);
	glMatrixMode(GL_MODELVIEW);
}

/// <summary>
/// A callback function, that serves as the timer function,
/// which controls how many times to redraw the screen during per second.
/// </summary>
/// <param name="(Not used): ">the value of the parameter to glutTimerFunc.</param>
void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, Timer, 0);
	static int c{};
	static int time{};
	if (c == 0 && _Engine.Time_Start)
	{
		c++;
	}
	else if (c > 0)
	{
		if (_Engine.Time_Start)
		{
			time++;
			if (time % 60 == 0)
			{
				B_Time.Sec++;
			}
		}
	}
}

/// <summary>
/// Initialization function.
/// </summary>
void init()
{
	glClearColor(0.68f, 0.68f, 0.68f, 1.0f);
	if (!_Engine.LoadFile(Load_In))
	{
		_Engine.Sound_On = true;
		_Engine.LoadFile(Load_Out); //Create file
	}
}

/// <summary>
/// A callback function that is responsible for catching any mouse click.
/// </summary>
/// <param name="button: ">The button on the mouse that was clicked.</param>
/// <param name="state: ">The state of the button on the mouse that was clicked.</param>
/// <param name="x: ">The X position of the click.</param>
/// <param name="y: ">The Y position of the click.</param>
void Mouse(int button, int state, int x, int y)
{
	float Cell_Pos_X{ (float)x / Pix_Cell_Size};
	float Cell_Pos_Y{ (float)Window_H / Pix_Cell_Size - ((float)y / Pix_Cell_Size) - 2 };
	if (_Engine.Game_End == false)
	{
		static int newx;
		static int newy;
		if (ActionCounter == 0 && _Engine.CellInMap(x, y))
		{
			_Engine.Time_Start = true; 
			ActionCounter++;
		}
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				newx = x;
				newy = y;
			}
			else if (state == GLUT_UP)
			{
				if (newx / Pix_Cell_Size == x / Pix_Cell_Size && newy / Pix_Cell_Size == y / Pix_Cell_Size)
				{
					if (_Engine.CellInMap(x, y))
					{
						if (Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].flag_checked == true) return;
						if (Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].open == true) return;

						_Engine.Open_Empty_Cells((int)Cell_Pos_X, (int)Cell_Pos_Y);

						if (Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].mine == true)
						{
							ActionCounter++;
							_Engine.Time_Start = false;
							_Engine.Game_End = true;
							return;
						}
					}
				}
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				newx = x;
				newy = y;
			}
			if (state == GLUT_UP)
			{
				if (_Engine.CellInMap(newx, newy) && Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].open == false)
				{
					if (newx / Pix_Cell_Size == x / Pix_Cell_Size && newy / Pix_Cell_Size == y / Pix_Cell_Size)
					{
						if (_Engine.Sound_On) PlaySound(L"Sounds/FlagPlaced.wav", NULL, SND_ASYNC | SND_NOSTOP);
						if (Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].flag_checked == false)
						{
							Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].flag_checked = true;
							_Engine.Mines_Left--;
							_Engine.Cells_used++;
						}
						else if (Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].flag_checked == true)
						{ 
							Cell[(int)Cell_Pos_X - 1][(int)Cell_Pos_Y - 1].flag_checked = false;
							_Engine.Mines_Left++;
							_Engine.Cells_used--;
						}
					}
				}
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (Cell_Pos_X >= B_EasyMode.X && Cell_Pos_X <= (B_EasyMode.X + B_EasyMode.Width))
			{
				if (Cell_Pos_Y >= B_EasyMode.Y && Cell_Pos_Y <= (B_EasyMode.Y + B_EasyMode.Height))
				{
					ActionCounter = 0;
					_Engine.StartGame(true, Mode_Easy);
				}
			}
			else if (Cell_Pos_X >= B_MediumMode.X && Cell_Pos_X <= (B_MediumMode.X + B_MediumMode.Width))
			{
				if (Cell_Pos_Y >= B_MediumMode.Y && Cell_Pos_Y <= (B_MediumMode.Y + B_MediumMode.Height))
				{
					ActionCounter = 0;
					_Engine.StartGame(true, Mode_Medium);
				}
			}
			else if (Cell_Pos_X >= B_HardMode.X && Cell_Pos_X <= (B_HardMode.X + B_HardMode.Width))
			{
				if (Cell_Pos_Y >= B_HardMode.Y && Cell_Pos_Y <= (B_HardMode.Y + B_HardMode.Height))
				{
					ActionCounter = 0;
					_Engine.StartGame(true, Mode_Hard);
				}
			}
			if (Cell_Pos_X >= Sound_Button.X && Cell_Pos_X <= (Sound_Button.X + Sound_Button.Width))
			{
				if (Cell_Pos_Y >= Sound_Button.Y && Cell_Pos_Y <= (Sound_Button.Y + Sound_Button.Height))
				{
					_Engine.Sound_On = !_Engine.Sound_On;
					_Engine.LoadFile(Load_Out);
				}
			}
		}
	}
}

/// <summary>
/// A callback function that is responsible for catching any key press on the keyboard.
/// </summary>
/// <param name="key: ">The key that was pressed on the keyboard.</param>
/// <param name="(Not used): ">The mouse's X location in window relative coordinates when the key was pressed.</param>
/// <param name="(Not used): ">The mouse's Y location in window relative coordinates when the key was pressed.</param>
void Keyboard(unsigned char key, int, int)
{
	switch (key)
	{
	case Key_ESC:
	{
		if (MessageBoxCentered(hwnd, L"Are you sure you want to quit MINER?", L"Quit game?", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL, Window_W, Window_H) == IDYES)
		{
			glutLeaveMainLoop();
		}
	}
	break;
	}
}
