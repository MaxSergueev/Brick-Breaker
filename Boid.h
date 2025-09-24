#pragma once
#include <raylib.h>
#include "Obstacles.h"

enum class BoidType {
	Purple,
	Tan,
	Green
};

class Boid
{
public:
	Boid();
	void Initialize(Texture texture, BoidType type);
	bool IsSameType(const Boid& other) const { return boidType == other.boidType; }
	Color GetColorForType(BoidType type) const;

	void Update(Boid flock[], const Obstacles& obstacleField, int const size);
	Vector2 Separate(Boid flock[], int const size);
	Vector2 AvoidObstacles(const Obstacles& obstacle);
	Vector2 Align(Boid flock[], int const size);
	Vector2 Group(Boid flock[], int const size);
	Vector2 Random(Boid flock[], int const size);

	Vector2 Hunt(Boid flock[], int const size);
	Vector2 Flee(Boid flock[], int const size);
	bool IsAlive = true; 

	BoidType boidType;
	Vector2 boidPosition = { 0, 0 };

private:
	Vector2 boidSpeed = { 1, 1 };
	Texture fish;

	float maxSpeed = 2.0f;
	float maxTurn = 20.0f;

	Color boidColor;

	float huntRadius = 150.0f;
	float fleeRadius = 100.0f;
	float catchRadius = 8.0f;

	BoidType GetPredator() const;
	BoidType GetPrey() const;

};

bool ColorsAreEqual(Color a, Color b);
