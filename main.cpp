#include "raylib.h"
#include <iostream>

using namespace std;

int main() {
    int screenWidth = 600;
    int screenHeight = 600;

    class Ball {
        public:
            int X;
            int Y;
            int speedX;
            int speedY;
    };

    Ball myBall;
    myBall.speedX = 5;
    myBall.speedY = 5;
    myBall.X = screenWidth / 3;
    myBall.Y = screenHeight / 2;

    class pong {
        public:
            int width;
            int height;

            int X;
            int Y;

    };

    pong myPong;
    myPong.width = screenWidth / 30;
    myPong.height = screenHeight / 100;
    myPong.X = screenHeight / 2;
    myPong.Y = screenWidth / 2;


    
     
    cout << "Hello World" << endl;

    InitWindow(screenWidth, screenHeight, "Purple window!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        myBall.X += myBall.speedX;
        myBall.Y += myBall.speedY;

        BeginDrawing();
        ClearBackground(DARKPURPLE);
        DrawCircle(myBall.X, myBall.Y, 50, GREEN);
        DrawRectangle(myPong.X, myPong.Y, myPong.width, myPong.height, RED);
        //DrawText("X coordinate is:" %int, 10, 10, 10, RED);
        DrawText(TextFormat("X coordinate is: %i", myBall.X), 20, 20, 20, RED);
        EndDrawing();

        if (myBall.X > screenWidth || myBall.X < 0) {
            myBall.speedX *= -1;
        }

        if (myBall.Y > screenWidth || myBall.Y < 0) {
            myBall.speedY *= -1;
        }
    }

    CloseWindow();
    return 0;
}