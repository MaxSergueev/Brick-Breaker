#pragma once
#include <raylib.h>
#include "Obstacles.h"

class Boid
{
public:
	Boid();
	void Initialize(Texture texture, Color boidColor);
	void Update(Boid flock[], const Obstacles& obstacleField, int const size);
	Vector2 Separate(Boid flock[], int const size);
	Vector2 AvoidObstacles(const Obstacles& obstacle);
	Vector2 Align(Boid flock[], int const size);
	Vector2 Group(Boid flock[], int const size);
	Vector2 Random(Boid flock[], int const size);

private:
	Vector2 boidPosition = {0, 0};
	Vector2 boidSpeed = { 1, 1 };
	Texture fish;

	float maxSpeed = 2.0f;
	float maxTurn = 20.0f;

	Color boidColor = WHITE;

};

bool ColorsAreEqual(Color a, Color b);
