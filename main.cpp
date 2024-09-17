#include "raylib.h"
#include <iostream>

using namespace std;

int main() {

    float screenWidth = 600;
    float screenHeight = 600;

    class Ball {
        public:
            Vector2 circ;
            int radius;
            int speedX;
            int speedY;
    };

    Ball myBall;
    myBall.circ = { screenWidth / 3, screenHeight / 2 };
    myBall.radius = 5;
    myBall.speedX = 5;
    myBall.speedY = 5;

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
        
        myBall.circ.x += myBall.speedX;
        myBall.circ.y += myBall.speedY;

        myPaddle.vect.x = GetMouseX() - myPaddle.vect.width/2;

        bool collision = CheckCollisionCircleRec(myBall.circ, myBall.radius, myPaddle.vect);

        BeginDrawing();
        ClearBackground(DARKPURPLE);

        DrawCircleV(myBall.circ, myBall.radius, GREEN);
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

        //Change ball's direction if it hits the border of the window
        if (myBall.circ.x > screenWidth || myBall.circ.x < 0) {
            myBall.speedX *= -1;
        }

        if (myBall.circ.y > screenWidth || myBall.circ.y < 0) {
            myBall.speedY *= -1;
        }

        if (collision) {
            myBall.speedY *= -1;
        }

        
    }

    CloseWindow();
    return 0;
}