#pragma once
#include "raylib.h"
class Ball{
public:
	Vector2 circ;
	int radius;
	int speedX;
	int speedY;

	Ball(float x, float y, int r, int sx, int sy);
	void Update();
	void Draw();
	void CheckPadCollision(Rectangle vect);
	void CheckWindowCollision(float screenWidth, float screenHeight);
};

