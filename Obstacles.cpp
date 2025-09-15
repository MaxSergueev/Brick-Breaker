#include "Obstacles.h"

Obstacles::Obstacles()
{

}

void Obstacles::addRectangle(Vector2 pos, int width, int height, Color color)
{
	Rectangle rect;
	rect.x = pos.x;
	rect.y = pos.y;
	rect.width = static_cast<float>(width);
	rect.height = static_cast<float>(height);

	obstacleList.push_back(rect);
	obstacleColors.push_back(color);
}
