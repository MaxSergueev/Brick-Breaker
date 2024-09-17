#pragma once
#include "raylib.h"
class Paddle{
public:
	Rectangle vect;

	Paddle(float x, float y, float width, float height);
	void Update(float screenWidth);
	void Draw();

};

