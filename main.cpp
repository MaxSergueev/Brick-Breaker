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
    myBall.radius = 10;
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