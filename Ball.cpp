#include "Ball.h"
#include "raylib.h"

//Coordinate vector, radius, and speed variables
Ball::Ball(float x, float y, int r, int sx, int sy) {
	circ = { x, y };
	radius = r;
	speedX = sx;
	speedY = sy;
}

//Ball movement
void Ball::Update() {
	circ.x += speedX;
	circ.y += speedY;
}

void Ball::Draw() {
	DrawCircleV(circ, radius, GREEN);
}

void Ball::CheckPadCollision(Rectangle vect){
	if (CheckCollisionCircleRec(circ, radius, vect))
	{

		speedX = (5*(circ.x - (vect.x + vect.width / 2)))/ (float)(vect.width/2);
		speedY *= -1;

	}
}

//Check collision with window's borders
void Ball::CheckWindowCollision(float screenWidth, float screenHeight) {
	if (circ.x > screenWidth || circ.x < 0) {
		speedX *= -1;
	}
	if (circ.y > screenHeight || circ.y < 0) {
		speedY *= -1;
	}
}