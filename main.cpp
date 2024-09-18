#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "ScoreManager.h"
#include "BrickGrid.h"
#include <iostream>

using namespace std;

typedef enum GameScreen { TITLE, GAMEPLAY, ENDING } GameScreen;

int main() {

    const float screenWidth = 600;
    const float screenHeight = 600;

    GameScreen currentScreen = TITLE;

    Ball myBall(screenWidth / 3, screenHeight / 2, 5, 5, 5);
    Paddle myPaddle(screenWidth / 2, screenHeight * 7 / 8, screenWidth / 8, screenHeight / 40);
    BrickGrid brickGrid(screenWidth, screenHeight);

    bool collision;

    InitWindow(screenWidth, screenHeight, "Brick Breaker");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        switch (currentScreen)
        {
        case TITLE:

            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
            }

            break;

        case GAMEPLAY:

            myPaddle.Update(screenWidth);
            myBall.Update();

            myBall.CheckWindowCollision(screenWidth, screenHeight);
            collision = CheckCollisionCircleRec(myBall.circ, myBall.radius, myPaddle.vect);

            if (brickGrid.CheckCollision(myBall.circ, myBall.radius) || collision) {
                myBall.speedY *= -1;
            }

            if (myBall.circ.y > screenHeight)
            {
                currentScreen = ENDING;
            }

            break;

        case ENDING:

            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }

            break;

        default: break;
        }

        BeginDrawing();

        ClearBackground(DARKPURPLE);

        switch (currentScreen)
        {
        case TITLE:
        {
         
            DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
            DrawText("BRICK BREAKER", screenWidth / 6, screenHeight * 0.36, 40, DARKGREEN);
            DrawText("PRESS ENTER", screenWidth / 6, screenHeight * 0.43, 20, DARKGREEN);

        } break;
        case GAMEPLAY:
        {
            myPaddle.Draw();
            myBall.Draw();
            brickGrid.Draw();

            DrawText(TextFormat("Score: %i", brickGrid.Score()), screenWidth * 0.05, screenHeight * 0.95, 20, RED);

        } break;
        case ENDING:
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER", 120, 220, 20, DARKBLUE);

        } break;
        default: break;
        }

        EndDrawing();

        
    }

    CloseWindow();
    return 0;
}