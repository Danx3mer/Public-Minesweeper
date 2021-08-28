#include "Label.h"
#include "Engine.h"

extern Engine _Engine;
extern std::fstream File;

/// <summary>
/// The default constructor for the label class.
/// </summary>
Label::Label() :Text((char*)""), Show_time(false), Sec(0), Min(0)
{
}

/// <summary>
/// A more detailed constructor for the label class.
/// </summary>
/// <param name="str: ">The text of the label.</param>
/// <param name="x: ">The X position of the label.</param>
/// <param name="y: ">The Y position of the label.</param>
/// <param name="w: ">The width of the label.</param>
/// <param name="h: ">The height of the label.</param>
/// <param name="t: ">Wether or not the label shows time.</param>
Label::Label(char* str, float x, float y, float w, float h, bool t) : Show_time(t), Text(str), Sec(0), Min(0)
{
	X = x;
	Y = y;
	Width = w;
	Height = h;
}

/// <summary>
/// Draws the labels.
/// </summary>
/// <param name="frame: ">Wether or not there should be a frame around the label.</param>
/// <param name="TX: ">The X coordinate of the text.</param>
/// <param name="TY: ">The Y coordinate of the text.</param>
/// <param name="TW: ">The Width of the text.</param>
/// <param name="TH: ">The Height of the text.</param>
/// <param name="Selected: ">Wether or not the button is selected (Only available for buttons Easy/Medium/Hard).</param>
/// <param name="LightColor: ">Specifies wether or not the color should be the same as backround.</param>
void Label::Draw(bool frame, float TX, float TY, float TW, float TH, bool Selected, bool LightColor)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	if (LightColor)	glColor3f(0.87f, 0.87f, 0.87f);
	glBegin(GL_QUADS);
	
	glVertex2f(X, Y);
	glVertex2f(X + Width, Y);
	glVertex2f(X + Width, Y + Height);
	glVertex2f(X, Y + Height);
	
	glEnd();

	if (frame)
	{
		glLineWidth(6.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex2f(X - 0.1f, Y);
		glVertex2f(X + Width + 0.1f, Y);

		glVertex2f(X + Width + 0.04f, Y);
		glVertex2f(X + Width + 0.04f, Y + Height);

		glVertex2f(X + Width + 0.1f, Y + Height);
		glVertex2f(X - 0.1f, Y + Height);

		glVertex2f(X - 0.04f, Y + Height);
		glVertex2f(X - 0.04f, Y);

		glEnd();
	}
	if (Selected)
	{
		glLineWidth(2.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);

		glVertex2f(X + 0.09f, Y + 0.09f);
		glVertex2f(X + Width - 0.13f, Y + 0.09f);

		glVertex2f(X + Width - 0.13f, Y + 0.09f);
		glVertex2f(X + Width - 0.13f, Y + Height - 0.1f);

		glVertex2f(X + Width - 0.13f, Y + Height - 0.1f);
		glVertex2f(X + 0.09f, Y + Height - 0.1f);

		glVertex2f(X + 0.09f, Y + Height - 0.1f);
		glVertex2f(X + 0.09f, Y + 0.1f);

		glEnd();
		glColor3f(0.0f, 0.0f, 0.0f);
	}
	if(Show_time) DrawTime(Sec, Min);
	_Engine.DrawTxt(Text, TX + TW / 2 - 1.0f, TY + TH / 2 - 0.15f);
}

/// <summary>
/// Draws the time on a label.
/// </summary>
/// <param name="seconds: ">Takes the time in seconds to draw it on the screen. Also converts seconds to minutes if over 59 seconds.</param>
/// <param name="minutes: ">Takes the time in minutes to draw it on the screen.</param>
void Label::DrawTime(int &seconds, int& minutes)
{
	for (seconds; seconds >= 60; seconds-=60)
	{
		minutes++;
	}
	_Engine.DrawNumber(minutes / 10, X + 0.9f, Y + 0.13f, false);
	_Engine.DrawNumber(minutes % 10, X + 1.15f, Y + 0.13f, false);
	_Engine.DrawTxt((char*)":", X + 1.45f, Y + 0.15f);
	_Engine.DrawNumber(seconds / 10, X + 1.6f, Y + 0.13f, false);
	_Engine.DrawNumber(seconds % 10, X + 1.85f, Y + 0.13f, false);
}

/// <summary>
/// Draws how many mines are still left on the game field.
/// </summary>
void Label::DrawMineCounter()
{
	float x = X;
	float y = Y;
	if (_Engine.Mines_Left < 0)
	{
		x += 0.2f;
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex2f(x + 0.14f, y + Height / 2 - 0.2f);
		glVertex2f(x + 0.4f, y + Height / 2 - 0.2f);

		glEnd();

		_Engine.DrawNumber(_Engine.Mines_Left / 10, x + 0.65f, y + 0.13f, false, true);
		_Engine.DrawNumber(_Engine.Mines_Left % 10, x + 0.9f, y + 0.13f, false, true);
		return;
	}
	_Engine.DrawNumber(_Engine.Mines_Left / 10, x + 0.75f, y + 0.13f, false);
	_Engine.DrawNumber(_Engine.Mines_Left % 10, x + 1.0f, y + 0.13f, false);
}

/// <summary>
/// Draws a sound button icon on a label.
/// </summary>
/// <param name="SoundOn: ">Specifies wether or not the sound of the game is on.</param>
void Label::DrawSoundButton(bool SoundOn)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	glVertex2f(X + 0.1f, Y + 0.6f);
	glVertex2f(X + 0.3f, Y + 0.6f);
	glVertex2f(X + 0.3f, Y + 0.2f);
	glVertex2f(X + 0.1f, Y + 0.2f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex2f(X + 0.1f, Y + 0.4f);
	glVertex2f(X + 0.55f, Y + 0.15f);
	glVertex2f(X + 0.55f, Y + 0.65f);

	glEnd();

	glLineWidth(3.0f);
	glBegin(GL_LINES);

	glVertex2f(X + 0.6f, Y + 0.5f);
	glVertex2f(X + 0.75f, Y + 0.6f);
	
	glVertex2f(X + 0.56f, Y + 0.4f);
	glVertex2f(X + 0.75f, Y + 0.4f);

	glVertex2f(X + 0.6f, Y + 0.3f);
	glVertex2f(X + 0.75f, Y + 0.2f);
	
	glEnd();

	if (SoundOn == false)
	{
		glColor3f(0.9f, 0.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex2f(X + 0.04f, Y + 0.06f);
		glVertex2f(X + Width - 0.03f, Y + Height - 0.06f);

		glEnd();
	}
}
