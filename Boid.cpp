#include "boid.h"
#include "raymath.h"

Boid::Boid()
{
}

void Boid::Initialize()
{
	float posX = GetRandomValue(10, GetScreenWidth() - 10);
	float posY = GetRandomValue(10, GetScreenHeight() - 10);
	boidPosition = {posX, posY};

	float speedX = GetRandomValue(-100, 100);
	float speedY = GetRandomValue(-100, 100);
	boidSpeed = { speedX / 100, speedY / 100};
}

void Boid::Update(Boid flock[], int const size)
{
	boidPosition.x += boidSpeed.x;
	boidPosition.y += boidSpeed.y;

	if ((boidPosition.x >= (GetScreenWidth() - 5)) || (boidPosition.x <= 5)) boidSpeed.x *= -1.0f;
	if ((boidPosition.y >= (GetScreenHeight() - 5)) || (boidPosition.y <= 5)) boidSpeed.y *= -1.0f;

	for (int i = 0; i < size; i++) {
		float dist = Vector2Distance(boidPosition, flock[i].boidPosition);
		if (dist < 20 && dist != 0) {
			boidSpeed.x *= -1;
			boidSpeed.y *= -1;
		}
	}

	DrawCircleV(boidPosition, 5, WHITE);
	DrawCircleLines(boidPosition.x, boidPosition.y, 10, RED);
}

Vector2 Boid::Separate(Boid[])
{
	return Vector2();
}

Vector2 Boid::AvoidObstacles(Obstacle[])
{
	return Vector2();
}

Vector2 Boid::Align(Boid[])
{
	return Vector2();
}

Vector2 Boid::Group(Boid[])
{
	return Vector2();
}
