#include "BrickGrid.h"

BrickGrid::BrickGrid(float screenWidth, float screenHeight)
	: brickWidth(screenWidth / 10), brickHeight(screenHeight / 40) { //didn't know you could do this, pretty cool

	int brickGridWidth = numBricks * brickWidth + (numBricks - 1) * spacing;
	brickMargin = (screenWidth - brickGridWidth) / 2;

	bricks.resize(numRows, std::vector<Brick>(numBricks));

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numBricks; col++) {
            float posX = brickMargin + col * (brickWidth + spacing);
            float posY = brickMargin + row * (brickHeight + spacing);
            bricks[row][col] = Brick(posX, posY, brickWidth, brickHeight);
        }
    }
}


void BrickGrid::Draw() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numBricks; col++) {
            if (bricks[row][col].isVisible) {
                bricks[row][col].Draw(); //Draws brick in the grid if it has not been hit
            }
        }
    }
}

bool BrickGrid::CheckCollision(Vector2 ballPosition, float ballRadius) {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numBricks; col++) {
            if (bricks[row][col].isVisible && CheckCollisionCircleRec(ballPosition, ballRadius, bricks[row][col].vect)) {
                bricks[row][col].isVisible = false; // Hide the brick
                return true;
                
            }
        }
    }
    return false;
}