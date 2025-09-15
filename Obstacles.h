#pragma once
#include <raylib.h>
#include <vector>

class Obstacles
{	
public:
	Obstacles();
	
	void addRectangle(Vector2 pos, int width, int height);

	std::vector<Rectangle> obstacleList;
};

