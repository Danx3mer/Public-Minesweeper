#pragma once
#include "Log.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

#define Window_W 600
#define Window_H 800

#define Load_Out false
#define Load_In true

#define Mode_Easy 10
#define Mode_Medium 20
#define Mode_Hard 30

#define Key_ESC 27

void display();
void Reshape(int W, int H);
void Timer(int);
void Mouse(int, int, int, int);
void Keyboard(unsigned char, int, int);
void init();
