#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "ScoreManager.h"
#include "BrickGrid.h"
#include <iostream>

using namespace std;

int main() {

    const float screenWidth = 600;
    const float screenHeight = 600;

    Ball myBall(screenWidth / 3, screenHeight / 2, 5, 5, 5);
    Paddle myPaddle(screenWidth / 2, screenHeight * 7 / 8, screenWidth / 8, screenHeight / 40);
    BrickGrid brickGrid(screenWidth, screenHeight);

    InitWindow(screenWidth, screenHeight, "Brick Breaker");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        myPaddle.Update(screenWidth);
        myBall.Update();

        bool collision = CheckCollisionCircleRec(myBall.circ, myBall.radius, myPaddle.vect);

        if (brickGrid.CheckCollision(myBall.circ, myBall.radius) || collision) {
            myBall.speedY *= -1;
        }


        BeginDrawing();
        ClearBackground(DARKPURPLE);

        myPaddle.Draw();
        myBall.Draw();
        brickGrid.Draw();

        DrawText(TextFormat("Score: %i", brickGrid.Score()), screenWidth * 0.05, screenHeight * 0.95, 20, RED);

        EndDrawing();

        myBall.CheckWindowCollision(screenWidth, screenHeight);

        
    }

    CloseWindow();
    return 0;
}