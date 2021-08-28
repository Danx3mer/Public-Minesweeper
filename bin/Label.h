#pragma once
#include "Rectangle.h"
#include <string>

class Label : public Rectangle
{
public:
	Label();
	Label(char* str, float x, float y, float w, float h, bool t);

	void Draw(bool frame, float TX, float TY, float TW, float TH, bool Popout = false, bool LightColor = false);
	void DrawTime(int&, int&);
	void DrawMineCounter();
	void DrawSoundButton(bool SoundOn);
public:
	char* Text;

	int Min;
	int Sec;
	bool Show_time;
};
