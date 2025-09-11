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

	float speedX = GetRandomValue(-200, 200);
	float speedY = GetRandomValue(-200, 200);
	boidSpeed = { speedX / 100, speedY / 100};
}

void Boid::Update(Boid flock[], int const size)
{
    // Calculate steering forces
    Vector2 separation = Vector2Scale(Separate(flock, size), 1.2f);
    Vector2 alignment = Vector2Scale(Align(flock, size), 1.0f);
    Vector2 cohesion = Vector2Scale(Group(flock, size), 1.0f);

    // Add momentum by blending speeds
    float momentum = 0.85f;
    Vector2 newSpeed = Vector2Add(boidSpeed, separation);
    newSpeed = Vector2Add(newSpeed, alignment);
    newSpeed = Vector2Add(newSpeed, cohesion);

    // Blend old and new speeds
    boidSpeed.x = boidSpeed.x * momentum + newSpeed.x * (1 - momentum);
    boidSpeed.y = boidSpeed.y * momentum + newSpeed.y * (1 - momentum);

    // Update position
    boidPosition = Vector2Add(boidPosition, boidSpeed);

    // Screen wrapping
    if (boidPosition.x > GetScreenWidth()) boidPosition.x = 0;
    if (boidPosition.x < 0) boidPosition.x = GetScreenWidth();
    if (boidPosition.y > GetScreenHeight()) boidPosition.y = 0;
    if (boidPosition.y < 0) boidPosition.y = GetScreenHeight();

    // Draw
    DrawCircleV(boidPosition, 5, WHITE);
	//DrawCircleLines(boidPosition.x, boidPosition.y, 10, RED); // For debugging
}

Vector2 Boid::Separate(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    int neighborCount = 0;
    float separationRadius = 15.0f;
    float maxForce = 0.5f;

    for (int i = 0; i < size; i++) {
        float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

        if (distance > 0 && distance < separationRadius) {
            Vector2 diff = Vector2Subtract(boidPosition, flock[i].boidPosition);
            diff = Vector2Scale(diff, 1.0f / (distance * distance));
            steering = Vector2Add(steering, diff);
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        // Average
        steering = Vector2Scale(steering, 1.0f / neighborCount);

        // Normalize and scale to max speed
        float length = Vector2Length(steering);
        if (length > 0) {
            steering = Vector2Scale(steering, maxForce / length);
        }
    }

    return steering;
}

Vector2 Boid::Align(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    int neighborCount = 0;
    float alignmentRadius = 20.0f;

    for (int i = 0; i < size; i++) {
        float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

        if (distance > 0 && distance < alignmentRadius) {
            steering = Vector2Add(steering, flock[i].boidSpeed);
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        steering = Vector2Scale(steering, 1.0f / neighborCount);
        steering = Vector2Subtract(steering, boidSpeed);
    }

    return steering;
}

Vector2 Boid::Group(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    int neighborCount = 0;
    float cohesionRadius = 30.0f;
    float maxForce = 0.3f;

    for (int i = 0; i < size; i++) {
        float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

        if (distance > 0 && distance < cohesionRadius) {
            steering = Vector2Add(steering, flock[i].boidPosition);
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        // Calculate center of mass
        steering = Vector2Scale(steering, 1.0f / neighborCount);

        // Create desired velocity vector toward center of mass
        Vector2 desired = Vector2Subtract(steering, boidPosition);
        float length = Vector2Length(desired);

        if (length > 0) {
            // Scale to max speed
            desired = Vector2Scale(desired, 2.0f / length);

            // Calculate steering force
            steering = Vector2Subtract(desired, boidSpeed);

            // Limit the steering force
            float steeringLength = Vector2Length(steering);
            if (steeringLength > maxForce) {
                steering = Vector2Scale(steering, maxForce / steeringLength);
            }
        }
    }

    return steering;
}


Vector2 Boid::AvoidObstacles(Obstacle[])
{
    return Vector2();
}


