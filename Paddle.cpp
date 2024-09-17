#include "Paddle.h"
#include "raylib.h"

Paddle::Paddle(float x, float y, float width, float height) {
	vect = { x, y, width, height };
}

void Paddle::Update(float screenWidth) {
	vect.x = GetMouseX() - vect.width / 2;
	if (vect.x < 0) vect.x = 0;
	if (vect.x + vect.width > screenWidth) vect.x = screenWidth - vect.width;
}

void Paddle::Draw() {
	DrawRectangleRec(vect, RED);
}