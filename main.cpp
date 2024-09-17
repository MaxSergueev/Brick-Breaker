#include "raylib.h"
#include "Ball.h"
#include <iostream>

using namespace std;

int main() {

    const float screenWidth = 600;
    const float screenHeight = 600;

    Ball myBall(screenWidth / 3, screenHeight / 2, 5, 5, 5);

    class Paddle {
        public:
            Rectangle vect;

    };

    Paddle myPaddle;
    myPaddle.vect.width = screenWidth / 8;
    myPaddle.vect.height = screenHeight / 40;
    myPaddle.vect.x = screenWidth / 2;
    myPaddle.vect.y = screenHeight * 7/8;

    class Brick {
        public:
            Rectangle vect;


    };
    const int spacing = 5;
    const int numRows = 10;
    const int numBricks = 8;
    const int brickWidth = screenWidth / 10;
    const int brickHeight = screenHeight / 40;

    //Calcluate total brick height, then calculate margin between bricks and borders
    int brickGridWidth = numBricks * brickWidth + (numBricks - 1) * spacing;
    int brickMargin = (screenWidth - brickGridWidth) / 2;

    //Visibility of bricks
    bool bricks[numRows][numBricks];
    for (int row = 0; row < numRows; row++) {
        for (int i = 0; i < numBricks; i++) {
            bricks[row][i] = true;
        }
    }




    InitWindow(screenWidth, screenHeight, "Purple window!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        myBall.Update();

        myPaddle.vect.x = GetMouseX() - myPaddle.vect.width/2;

        bool collision = CheckCollisionCircleRec(myBall.circ, myBall.radius, myPaddle.vect);

        BeginDrawing();
        ClearBackground(DARKPURPLE);

        myBall.Draw();
        DrawRectangleRec(myPaddle.vect, RED);

        for (int row = 0; row < numRows; row++) {
            for (int i = 0; i < numBricks; i++) {
                if(bricks[row][i]){

                    int posX = brickMargin + i * (brickWidth + spacing);
                    int posY = brickMargin + row * (brickHeight + spacing);

                    Rectangle rect = { posX, posY, brickWidth, brickHeight };
                    DrawRectangleRec(rect, MAROON);

                    if (CheckCollisionCircleRec(myBall.circ, myBall.radius, rect)) {
                        bricks[row][i] = false;
                        myBall.speedY *= -1;
                    }
                }
            }
        }

        DrawText(TextFormat("X coordinate is: %i", myBall.circ.x), screenWidth * 0.05, screenHeight * 0.95, 20, RED);

        EndDrawing();

        if (collision) {
            myBall.speedY *= -1;
        }

        myBall.CheckWindowCollision(screenWidth, screenHeight);

        
    }

    CloseWindow();
    return 0;
}