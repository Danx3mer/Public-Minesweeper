#pragma once
#include "Rectangle.h"

extern LOG Log;

class cell : public Rectangle
{
public:
	cell();
	void DrawFilledMine(float cx, float cy, float r, int num_segments);
	void DrawFlag();
	void Draw();
public:
	bool open;
	bool mine;
	bool flag_checked;
	int number;
};
