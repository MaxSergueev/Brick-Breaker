#pragma once
#include <raylib.h>
#include <vector>

class Obstacles
{	
public:
	Obstacles();
	
	void addRectangle(Vector2 pos, int width, int height, Color color);

	std::vector<Rectangle> obstacleList;
	std::vector<Color> obstacleColors;
};

