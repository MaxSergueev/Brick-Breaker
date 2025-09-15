#include "raylib.h"
#include "Boid.h"
#include <iostream>

using namespace std;
float width = 1280;
float height = 720;

int const boidTotal = 333;

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


    // Purple anemone
    obstacles.addRectangle(Vector2{ width / 2 + 50, height / 2 }, 200, 100, Color{ 202, 78, 121, 200 });
    obstacles.addRectangle(Vector2{ width / 2 - 50, height / 2 + 100}, 200, 100, Color{ 202, 78, 121, 200 });
    obstacles.addRectangle(Vector2{ width / 2 - 150, height / 2 + 100 }, 100, 200, Color{ 202, 78, 121, 200 });

    // Tan Anemone
    obstacles.addRectangle(Vector2{ width / 2 - 500, height / 8.0f }, 300, 100, Color{ 255, 193, 142, 200 });
    obstacles.addRectangle(Vector2{ width / 2 - 600, height / 8.0f + 100 }, 200, 100, Color{ 255, 193, 142, 200 });
    obstacles.addRectangle(Vector2{ width / 2 - 200, height / 8.0f }, 100, 200, Color{ 255, 193, 142, 200 });

    // Orange Anemone
    obstacles.addRectangle(Vector2{ width / 2 + 250, height / 8.0f }, 200, 100, Color{ 229, 80, 80, 200 });
    obstacles.addRectangle(Vector2{ width / 2 + 300, height / 8.0f + 100 }, 200, 100, Color{ 229, 80, 80, 200 });


    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int i = 0; i < size(obstacles.obstacleList); i++) {

            DrawRectangle(obstacles.obstacleList[i].x,
                obstacles.obstacleList[i].y,
                obstacles.obstacleList[i].width,                     // grr
                obstacles.obstacleList[i].height,
                obstacles.obstacleColors[i]);
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