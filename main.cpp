#include "raylib.h"
#include "Boid.h"
#include <iostream>

using namespace std;
float width = 1280;
float height = 720;

int const boidTotal = 900;

Boid boids[boidTotal];
Obstacles obstacles;

int main() {

    InitWindow(width, height, "Boids Mania");
    SetTargetFPS(60);

    Image fish = LoadImage("resources/WhiteFish.png");
    ImageResize(&fish, 15, 15);
    Texture2D texture = LoadTextureFromImage(fish);
    UnloadImage(fish);

    for (int i = 0; i < size(boids); i++) {
        int third = boidTotal / 3;
        if (i < third) {
            boids[i].Initialize(texture, Color{ 202, 78, 121, 255});
        }
        else if(third < i && i < 2 * third){
            boids[i].Initialize(texture, Color{ 255, 193, 142, 255});
        }
        else {
            boids[i].Initialize(texture, Color{ 229, 80, 80, 255 });
        }
    }

    obstacles.addRectangle(Vector2{ width / 2 + 50, height / 2 }, 200, 100);
    obstacles.addRectangle(Vector2{ width / 2 - 50, height / 2 + 100}, 200, 100);
    obstacles.addRectangle(Vector2{ width / 2 - 150, height / 2 + 100 }, 100, 200);

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (const Rectangle& rect : obstacles.obstacleList)
        {
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, Color{ 122, 64, 105, 255});
        }

        for (int i = 0; i < size(boids); i++) {
            boids[i].Update(boids, obstacles, boidTotal);
        }

        ClearBackground(Color{81, 50, 82, 255 });
        EndDrawing();
    }

    CloseWindow();
    return 0;
}