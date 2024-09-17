#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <iostream>

using namespace std;

int main() {

    const float screenWidth = 600;
    const float screenHeight = 600;

    Ball myBall(screenWidth / 3, screenHeight / 2, 5, 5, 5);
    Paddle myPaddle(screenWidth / 2, screenHeight * 7 / 8, screenWidth / 8, screenHeight / 40);

    //Brick grid variables/////////////////////////////////////////////////////////////
    const int spacing = 5;
    const int numRows = 10;
    const int numBricks = 8;
    const int brickWidth = screenWidth / 10;
    const int brickHeight = screenHeight / 40;

    //Calcluate total brick height, then calculate margin between bricks and borders
    int brickGridWidth = numBricks * brickWidth + (numBricks - 1) * spacing;
    int brickMargin = (screenWidth - brickGridWidth) / 2;

    //Initialise brick grid
    Brick bricks[numRows][numBricks];
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numBricks; col++) {
            float posX = brickMargin + col * (brickWidth + spacing);
            float posY = brickMargin + row * (brickHeight + spacing);
            bricks[row][col] = Brick(posX, posY, brickWidth, brickHeight);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////


    InitWindow(screenWidth, screenHeight, "Brick Breaker");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        myPaddle.Update(screenWidth);
        myBall.Update();

        // Check ball collision with the bricks //////////////////////////////////////
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numBricks; col++) {
                if (bricks[row][col].CheckCollision(myBall.circ, myBall.radius)) {
                    myBall.speedY *= -1;
                }
            }
        }
        /////////////////////////////////////////////////////////////////////////////

        bool collision = CheckCollisionCircleRec(myBall.circ, myBall.radius, myPaddle.vect);

        BeginDrawing();
        ClearBackground(DARKPURPLE);

        myPaddle.Draw();
        myBall.Draw();

        ////////////////////////////////////////////////////////////////////////////
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numBricks; col++) {
                bricks[row][col].Draw();
            }
        }
        ////////////////////////////////////////////////////////////////////////////

        DrawText(TextFormat("X coordinate is: %f", myBall.circ.x), screenWidth * 0.05, screenHeight * 0.95, 20, RED);

        EndDrawing();

        if (collision) {
            myBall.speedY *= -1;
        }

        myBall.CheckWindowCollision(screenWidth, screenHeight);

        
    }

    CloseWindow();
    return 0;
}