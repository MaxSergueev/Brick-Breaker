#include "boid.h"
#include "raymath.h"
#include "math.h"
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>

template <typename T>
T clamp(T value, T minVal, T maxVal)
{
    return std::max(minVal, std::min(value, maxVal));
}

bool ColorsAreEqual(Color a, Color b)
{
    return (a.r == b.r) &&
        (a.g == b.g) &&
        (a.b == b.b);
}

Boid::Boid()
{
}

void Boid::Initialize(Texture texture, BoidType type)
{
    fish = texture;
    boidType = type;
    boidColor = GetColorForType(type);

    float posX = GetRandomValue(10, GetScreenWidth() - 10);
    float posY = GetRandomValue(10, GetScreenHeight() - 10);
    boidPosition = { posX, posY };

    float speedX = GetRandomValue(-200, 200);
    float speedY = GetRandomValue(-200, 200);
    boidSpeed = { speedX / 100, speedY / 100 };
}

Color Boid::GetColorForType(BoidType type) const {
    switch (type) {
    case BoidType::Purple:
        return Color{ 202, 78, 121, 255 };
    case BoidType::Tan:
        return Color{ 255, 193, 142, 255 };
    case BoidType::Green:
        return Color{ 131, 252, 107, 255 };
    default:
        return WHITE;
    }
}

void Boid::Update(Boid flock[], const Obstacles& obstacleField, int const size)
{
    if (!IsAlive) return;

    // Base behaviors
    Vector2 separation = Vector2Scale(Separate(flock, size), 4.0f);
    Vector2 alignment = Vector2Scale(Align(flock, size), 1.0f);
    Vector2 cohesion = Vector2Scale(Group(flock, size), 3.5f);
    Vector2 obstacles = Vector2Scale(AvoidObstacles(obstacleField), 1000.0f);
    Vector2 random = Vector2Scale(Random(flock, size), 0.05f);

    // Type-specific behaviors
    Vector2 hunt = { 0, 0 };
    Vector2 flee = { 0, 0 };

    switch (boidType) {
    case BoidType::Purple:  // Highly avoidant
        hunt = Vector2Scale(Hunt(flock, size), 1.0f);     
        flee = Vector2Scale(Flee(flock, size), 12.0f);    
        separation = Vector2Scale(separation, 2.0f);      
        maxSpeed = 3.0f;                                  
        maxTurn = 30.0f;                                  
        break;

    case BoidType::Tan:     // Aggressive
        hunt = Vector2Scale(Hunt(flock, size), 10.0f);    
        flee = Vector2Scale(Flee(flock, size), 1.0f);     
        cohesion = Vector2Scale(cohesion, 1.5f);          
        maxSpeed = 2.8f;                                  
        maxTurn = 25.0f;                                  
        break;

    case BoidType::Green:   // Balanced
        hunt = Vector2Scale(Hunt(flock, size), 5.0f);     
        flee = Vector2Scale(Flee(flock, size), 5.0f);     
        maxSpeed = 2.5f;                                  
        maxTurn = 20.0f;                                  
        break;
    }

    // Combine all behaviors
    Vector2 desiredVelocity = Vector2Add(separation, alignment);
    desiredVelocity = Vector2Add(desiredVelocity, cohesion);
    desiredVelocity = Vector2Add(desiredVelocity, obstacles);
    desiredVelocity = Vector2Add(desiredVelocity, random);
    desiredVelocity = Vector2Add(desiredVelocity, hunt);
    desiredVelocity = Vector2Add(desiredVelocity, flee);

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
        float clampedAngle = turnAngle * DEG2RAD;
        steeringForce = Vector2Rotate(Vector2Scale(Vector2Normalize(boidSpeed),
            Vector2Length(steeringForce)), clampedAngle);
    }

    // Apply steering force
    boidSpeed = Vector2Add(boidSpeed, Vector2Scale(steeringForce, 0.15f));

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
    DrawTextureEx(fish, boidPosition, rot, 1, boidColor);
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

        if (distance > 0 && distance < alignmentRadius && IsSameType(flock[i])) {
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
    float cohesionRadius = 50.0f;
    float maxForce = 0.3f;

    for (int i = 0; i < size; i++) {
        float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

        if (distance > 0 && distance < cohesionRadius && IsSameType(flock[i])) {
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


Vector2 Boid::AvoidObstacles(const Obstacles& obstacleField)
{
    Vector2 steering = { 0.0f, 0.0f };
    float detectionRadius = 45.0f;
    float maxAvoidForce = 10.0f;

    for (size_t i = 0; i < obstacleField.obstacleList.size(); i++)
    {
        const Rectangle& rect = obstacleField.obstacleList[i];
        const Color& obstacleColor = obstacleField.obstacleColors[i];

        // Skip collision check if colors match (boid can enter its own color anemone)
        if (ColorsAreEqual(boidColor, obstacleColor))
            continue;

        if (CheckCollisionCircleRec(boidPosition, detectionRadius, rect))
        {
            float closestX = clamp(boidPosition.x, rect.x, rect.x + rect.width);
            float closestY = clamp(boidPosition.y, rect.y, rect.y + rect.height);
            Vector2 closestPoint = { closestX, closestY };

            // Vector pointing away from the obstacle
            Vector2 away = Vector2Subtract(boidPosition, closestPoint);

            // Inverse-square scaling based on proximity
            float dist = Vector2Length(away);
            if (dist > 0)
            {
                Vector2 scaledAway = Vector2Scale(away, 1.0f / (dist * dist));
                steering = Vector2Add(steering, scaledAway);
            }
        }
    }

    float steerLen = Vector2Length(steering);
    if (steerLen > maxAvoidForce)
    {
        steering = Vector2Scale(steering, maxAvoidForce / steerLen);
    }

    return steering;
}

BoidType Boid::GetPredator() const {
    switch (boidType) {
    case BoidType::Purple: return BoidType::Tan;      // Tan eats Purple
    case BoidType::Tan: return BoidType::Green;       // Green eats Tan
    case BoidType::Green: return BoidType::Purple;    // Purple eats Green
    default: return boidType;
    }
}

BoidType Boid::GetPrey() const {
    switch (boidType) {
    case BoidType::Purple: return BoidType::Green;    // Purple eats Green
    case BoidType::Tan: return BoidType::Purple;      // Tan eats Purple
    case BoidType::Green: return BoidType::Tan;       // Green eats Tan
    default: return boidType;
    }
}

Vector2 Boid::Hunt(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    float closestDist = huntRadius;
    Vector2 closestPrey = { 0.0f, 0.0f };
    bool foundPrey = false;

    for (int i = 0; i < size; i++) {
        if (!flock[i].IsAlive) continue; 

        if (flock[i].boidType == GetPrey()) {
            float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

            if (distance < catchRadius) {
                flock[i].IsAlive = false;
                continue;
            }

            if (distance < closestDist) {
                closestDist = distance;
                closestPrey = flock[i].boidPosition;
                foundPrey = true;
            }
        }
    }

    if (foundPrey) {
        Vector2 desired = Vector2Subtract(closestPrey, boidPosition);
        desired = Vector2Scale(Vector2Normalize(desired), maxSpeed);
        steering = Vector2Subtract(desired, boidSpeed);
    }

    return steering;
}

Vector2 Boid::Flee(Boid flock[], int const size)
{
    Vector2 steering = { 0.0f, 0.0f };
    int predatorCount = 0;

    for (int i = 0; i < size; i++) {
        if (!flock[i].IsAlive) continue;

        if (flock[i].boidType == GetPredator()) {
            float distance = Vector2Distance(boidPosition, flock[i].boidPosition);

            if (distance < fleeRadius) {
                Vector2 diff = Vector2Subtract(boidPosition, flock[i].boidPosition);
                diff = Vector2Scale(diff, 1.0f / (distance * distance));
                steering = Vector2Add(steering, diff);
                predatorCount++;
            }
        }
    }

    if (predatorCount > 0) {
        steering = Vector2Scale(steering, 1.0f / predatorCount);
        float length = Vector2Length(steering);
        if (length > 0) {
            steering = Vector2Scale(Vector2Normalize(steering), maxSpeed);
        }
    }

    return steering;
}



