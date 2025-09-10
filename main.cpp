#include "raylib.h"
#include "Boid.h"
#include <iostream>

using namespace std;
int width = 1280;
int height = 720;

int const boidTotal = 100;

Boid boids[boidTotal];

int main() {

    InitWindow(width, height, "My first Raylib window!");
    SetTargetFPS(60);
    for (int i = 0; i < size(boids); i++) {
        boids[i].Initialize();
    }

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int i = 0; i < size(boids); i++) {
            boids[i].Update(boids, boidTotal);
        }

        ClearBackground(SKYBLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}