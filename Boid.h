#pragma once
#include <raylib.h>
#include "Obstacle.h"

class Boid
{
public:
	Boid();
	void Initialize();
	void Update(Boid flock[], int const size);
	Vector2 Separate(Boid flock[], int const size);
	Vector2 AvoidObstacles(Obstacle[]);
	Vector2 Align(Boid flock[], int const size);
	Vector2 Group(Boid flock[], int const size);

private:
	Vector2 boidPosition = {0, 0};
	Vector2 boidSpeed = { 1, 1 };

};

