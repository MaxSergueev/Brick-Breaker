#include "Brick.h"
#include "raylib.h"

Brick::Brick(float x, float y, float width, float height) {
	vect = { x, y, width, height };
	isVisible = true;
}

void Brick::Draw(){
	if (isVisible) DrawRectangleRec(vect, MAROON);
}

bool Brick::CheckCollision(Vector2 ballPosition, float ballRadius) {
	if (isVisible && CheckCollisionCircleRec(ballPosition, ballRadius, vect)) {
		isVisible = false;
		return true;
	}
	return false;
}