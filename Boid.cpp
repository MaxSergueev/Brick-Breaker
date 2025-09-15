#include "boid.h"
#include "raymath.h"
#include "math.h"
#include <cmath>
#include <iostream>
#include <string>

Boid::Boid()
{
}

void Boid::Initialize(Texture texture)
{
    fish = texture;
	float posX = GetRandomValue(10, GetScreenWidth() - 10);
	float posY = GetRandomValue(10, GetScreenHeight() - 10);
	boidPosition = {posX, posY};

	float speedX = GetRandomValue(-200, 200);
	float speedY = GetRandomValue(-200, 200);
	boidSpeed = { speedX / 100, speedY / 100};
}

void Boid::Update(Boid flock[], int const size)
{
    // Calculate desired velocities for each behavior
    Vector2 separation = Vector2Scale(Separate(flock, size), 2.5f);
    Vector2 alignment = Vector2Scale(Align(flock, size), 1.0f);
    Vector2 cohesion = Vector2Scale(Group(flock, size), 5.5f);
    Vector2 random = Vector2Scale(Random(flock, size), 0.1f);

    // Combine all desired velocities
    Vector2 desiredVelocity = Vector2Add(separation, alignment);
    desiredVelocity = Vector2Add(desiredVelocity, cohesion);
    desiredVelocity = Vector2Add(desiredVelocity, random);

    // Normalize and scale to max speed
    if (Vector2Length(desiredVelocity) > 0) {
        desiredVelocity = Vector2Scale(
            Vector2Normalize(desiredVelocity),
            maxSpeed
        );
    }

    // Calculate steering force (desired velocity - current velocity)
    Vector2 steeringForce = Vector2Subtract(desiredVelocity, boidSpeed);

    // Calculate current and desired heading angles
    float currentHeading = atan2f(boidSpeed.y, boidSpeed.x);
    float desiredHeading = atan2f(desiredVelocity.y, desiredVelocity.x);

    // Calculate the shortest turning angle
    float turnAngle = desiredHeading - currentHeading;

    // Normalize angle to [-pi, pi]
    while (turnAngle > PI) turnAngle -= 2 * PI;
    while (turnAngle < -PI) turnAngle += 2 * PI;

    // Convert to degrees for maxTurn comparison
    turnAngle *= RAD2DEG;

    // Clamp turn angle
    if (fabs(turnAngle) > maxTurn) {
        turnAngle = (turnAngle > 0 ? maxTurn : -maxTurn);
        // Convert back to radians for rotation
        float clampedAngle = turnAngle * DEG2RAD;
        steeringForce = Vector2Rotate(Vector2Scale(Vector2Normalize(boidSpeed),
            Vector2Length(steeringForce)), clampedAngle);
    }

    // Apply steering force
    boidSpeed = Vector2Add(boidSpeed, Vector2Scale(steeringForce, 0.16f));

    // Ensure minimum speed
    float currentSpeed = Vector2Length(boidSpeed);
    if (currentSpeed < maxSpeed * 0.5f) {
        boidSpeed = Vector2Scale(Vector2Normalize(boidSpeed), maxSpeed * 0.5f);
    }
    // Limit maximum speed
    else if (currentSpeed > maxSpeed) {
        boidSpeed = Vector2Scale(Vector2Normalize(boidSpeed), maxSpeed);
    }

    // Update position
    boidPosition = Vector2Add(boidPosition, boidSpeed);

    // Screen wrapping
    if (boidPosition.x > GetScreenWidth()) boidPosition.x = 0;
    if (boidPosition.x < 0) boidPosition.x = GetScreenWidth();
    if (boidPosition.y > GetScreenHeight()) boidPosition.y = 0;
    if (boidPosition.y < 0) boidPosition.y = GetScreenHeight();

    // Calculate rotation for drawing
    float rot = Vector2Angle(boidSpeed, Vector2{ -1, 0 }) * RAD2DEG;
    DrawTextureEx(fish, boidPosition, rot, 1, WHITE);
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
    float alignmentRadius = 50.0f;
    float maxForce = 0.3f;

    for (int i = 0; i < size; i++) {
        float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

        if (distance > 0 && distance < alignmentRadius) {
            steering = Vector2Add(steering, flock[i].boidSpeed);
            neighborCount++;
        }
    }

    if (neighborCount > 0) {
        // Calculate average velocity
        steering = Vector2Scale(steering, 1.0f / neighborCount);
        
        // Create desired velocity
        float length = Vector2Length(steering);
        if (length > 0) {
            steering = Vector2Scale(steering, maxSpeed / length);
        }
        
        // Calculate steering force
        steering = Vector2Subtract(steering, boidSpeed);
        
        // Limit steering force
        float steerLength = Vector2Length(steering);
        if (steerLength > maxForce) {
            steering = Vector2Scale(steering, maxForce / steerLength);
        }
    }

    return steering;
}

Vector2 Boid::Group(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    int neighborCount = 0;
    float cohesionRadius = 30.0f;
    float maxForce = 0.2f;

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

Vector2 Boid::Random(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    float maxForce = 2.0f;

    steering.x = GetRandomValue(0, 100) / 100;
    steering.y = GetRandomValue(0, 100) / 100;

    float length = Vector2Length(steering);
    if (length > 0) {
        steering = Vector2Scale(steering, maxForce / length);

    }
    return steering;
}


Vector2 Boid::AvoidObstacles(Obstacle[])
{
    return Vector2();
}


