#pragma once
#include <raylib.h>
#include "Obstacle.h"

class Boid
{
public:
	Boid();
	void Initialize();
	void Update(Boid flock[], int const size);
	Vector2 Separate(Boid flock[]);
	Vector2 AvoidObstacles(Obstacle[]);
	Vector2 Align(Boid flock[]);
	Vector2 Group(Boid flock[]);

private:
	Vector2 boidPosition = {0, 0};
	Vector2 boidSpeed = { 1, 1 };

};

