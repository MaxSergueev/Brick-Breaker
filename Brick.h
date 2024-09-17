#pragma once
#include "raylib.h"
class Brick {
public:
	Rectangle vect;
	Brick() : vect{ 0, 0, 0, 0 }, isVisible(true) {}

	bool isVisible;

	Brick(float x, float y, float width, float height);
	void Draw();
	bool CheckCollision(Vector2 ballPosition, float ballRadius);

};

