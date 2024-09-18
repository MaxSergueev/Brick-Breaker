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
    
    InitWindow(screenWidth, screenHeight, "Brick Breaker");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        switch (currentScreen)
        {
        case TITLE: 
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
            }

            break;
        }

            

        case GAMEPLAY: 
        {

            myPaddle.Update(screenWidth);
            myBall.Update();

            myBall.CheckWindowCollision(screenWidth, screenHeight);
            myBall.CheckPadCollision(myPaddle.vect);

            if (brickGrid.CheckCollision(myBall.circ, myBall.radius)) {
                myBall.speedY *= -1;
            }

            if (myBall.circ.y > screenHeight)
            {
                currentScreen = ENDING;
            }

            //check if bricks are gone
            if (brickGrid.Score() == 800) 
            { 
                currentScreen = ENDING;
                myBall.speedY *= -1;
            }

            break;
        }

        case ENDING: 
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
                brickGrid.Reset();
                myBall.circ = { screenWidth / 2, screenHeight / 2 };
            }

            break;
        }

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
            if (brickGrid.Score() == 800) {
                DrawText(TextFormat("%s", "VICTORY!"), screenWidth / 12, screenHeight * 0.16, 60, DARKBLUE);
            }
            else {
                DrawText(TextFormat("%s", "GAME OVER"), screenWidth / 12, screenHeight * 0.16, 60, DARKBLUE);
            }
            DrawText(TextFormat("Score: %i", brickGrid.Score()), screenWidth / 5, screenHeight * 0.25, 40, RED);
            DrawText("PRESS ENTER TO RETURN TO TITLE", screenWidth / 6, screenHeight * 0.43, 20, DARKBLUE);

        } break;
        default: break;
        }

        EndDrawing();

        
    }

    CloseWindow();
    return 0;
}