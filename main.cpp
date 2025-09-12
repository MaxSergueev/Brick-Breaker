#include "raylib.h"
#include "Boid.h"
#include <iostream>

using namespace std;
int width = 1280;
int height = 720;

int const boidTotal = 100;

Boid boids[boidTotal];

int main() {

    InitWindow(width, height, "Boids Mania");
    SetTargetFPS(60);

    Image fish = LoadImage("resources/WhiteFish.png");
    ImageResize(&fish, 15, 15);
    Texture2D texture = LoadTextureFromImage(fish);
    UnloadImage(fish);

    for (int i = 0; i < size(boids); i++) {
        boids[i].Initialize(texture);
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