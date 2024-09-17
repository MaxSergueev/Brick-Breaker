#pragma once
#include "Brick.h"
#include "raylib.h"
#include <vector>
class BrickGrid{
public:
	BrickGrid(float screenWidth, float screenHeight);
	bool CheckCollision(Vector2 ballPosition, float ballRadius);
	void Draw();

private:
    const int spacing = 5;
    const int numRows = 10;
    const int numBricks = 8;
    const int brickWidth;
    const int brickHeight;

    int brickMargin;
    std::vector<std::vector<Brick>> bricks; // 2D vector of Brick objects

};

