#include "cell.h"
#include "Engine.h"

extern const int Pix_Cell_Size;

extern Engine _Engine;

/// <summary>
/// Default constructor for cell class.
/// </summary>
cell::cell() :mine(false), open(false), flag_checked(false), number(0)
{
	Rectangle::Rectangle();
}

/// <summary>
/// Draws a cell.
/// </summary>
void cell::Draw()
{
	if (!open)
		glColor3f(0.0f, 1.0f, 0.0f);
	if (open)
		glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POLYGON);

	glVertex2f(X, Y);
	glVertex2f(X + Width, Y);
	glVertex2f(X + Width, Y + Height);
	glVertex2f(X, Y + Height);

	glLineWidth(10.0f);

	glEnd();

	glLineWidth(10.0f);
	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex2f(X - 0.1f, 0.8f);
	glVertex2f(X - 0.1f, 11.0f);

	glVertex2f(0.8f, Y + 0.9f);
	glVertex2f(11.0f, Y + 0.9f);

	glVertex2f(X + 0.9f, 0.8f);
	glVertex2f(X + 0.9f, 11.0f);

	glVertex2f(0.8f, Y - 0.1f);
	glVertex2f(11.0f, Y - 0.1f);

	glEnd();
	if (number > 0 && open && !mine)
	{
		_Engine.DrawNumber(number, X + Width / 2 - 0.15f, Y + Height / 2 - 0.15f);
	}
	else if (!open && flag_checked)
	{
		DrawFlag();
	}
	else if (open && mine)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		double Mine_Radius{ 0.25f };
		DrawFilledMine(X + 0.4f, Y + 0.4f, Mine_Radius, Mine_Radius*50);
	}
}

/// <summary>
/// Draws a flag.
/// </summary>
void cell::DrawFlag()
{
	//flag stand
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex2f(X + 0.25f, Y + 0.1f);
	glVertex2f(X + 0.5f, Y + 0.1f);

	glVertex2f(X + 0.39f, Y + 0.1f);
	glVertex2f(X + 0.39f, Y + 0.6f);

	glEnd();

	//flag triangle
	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex2f(X + 0.37f, Y + 0.6f);
	glVertex2f(X + 0.19f, Y + 0.45f);
	glVertex2f(X + 0.37f, Y + 0.3f);
	
	glEnd();
}

/// <summary>
/// Draws a mine.
/// </summary>
/// <param name="cx: ">The X coordinate of the mine.</param>
/// <param name="cy: ">The Y coordinate of the mine.</param>
/// <param name="radius: ">The radius of the main part of the mine.</param>
/// <param name="num_segments: ">The number of segments(sides) to be drawn.</param>
void cell::DrawFilledMine(float cx, float cy, float radius, int num_segments)
{
	//Draw main part
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < num_segments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); //get the current angle

		float x = radius * cosf(theta); //calculate current x in the segment
		float y = radius * sinf(theta); //calculate current y in the segment

		glVertex2f(x + cx, y + cy); //output vertex
	}
	glEnd();

	//Draw thread
	glLineWidth(4.0f);
	glBegin(GL_LINES);

	glVertex2f(cx, cy);
	glVertex2f(cx + 0.25f, cy + 0.25f);

	glEnd();

	//draw flame
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_segments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle

		float x = radius/4 * cosf(theta); //calculate x
		float y = radius/4 * sinf(theta); //calculate y

		glVertex2f(x + cx + 0.25f, y + cy + 0.25f);//output vertex
	}
	glEnd();
}
