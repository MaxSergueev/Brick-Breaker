#include "raylib.h"
#include <vector>

// Slow down
#include <thread>
#include <chrono>

// Struct to represent a grid cell
struct Cell {
    int x, y;
    float f = 0, g = 0, h = 0;
    int weight = 1; // 1 = normal, 2 = twice, 3 = three times, -1 = impassable
    Cell* parent = nullptr;

    bool isWalkable() const {
        return weight != -1;
    }
};

// Grid and screen constants
const int GRID_SIZE = 50;
const int CELL_SIZE = 20;
const int SCREEN_WIDTH = GRID_SIZE * CELL_SIZE;
const int SCREEN_HEIGHT = GRID_SIZE * CELL_SIZE;

// Map Drawing Variables
bool isDragging = false;
Vector2 lastModifiedCell = { -1, -1 }; // Track the last modified cell to avoid redundant changes

// Directions for neighbors
std::vector<Vector2> directions = {
    {-1,  1}, {0,  1}, {1,  1}, 
    {-1,  0}, {0,  0}, {1,  0},
	{-1, -1}, {0, -1}, {1, -1}
};

// Calculate Manhattan distance
float ManhattanDistance(const Cell& a, const Cell& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// A* Pathfinding function
std::vector<Cell*> FindPath(Cell* start, Cell* goal, std::vector<std::vector<Cell>>& grid, std::vector<Cell*>& openListDraw, std::vector<Cell*>& closedListDraw) {
    std::vector<Cell*> openList;
    std::vector<Cell*> closedList;
    openList.push_back(start);

    while (!openList.empty()) {
        // Find the cell with the lowest f value
        auto currentIt = std::min_element(openList.begin(), openList.end(), [](Cell* a, Cell* b) { return a->f < b->f; });
        Cell* current = *currentIt;

        openList.erase(currentIt);
        closedList.push_back(current);

        // Update drawing lists
        openListDraw = openList;
        closedListDraw = closedList;

        // Render each step
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                Color color;
                switch (grid[x][y].weight) {
                case 1: color = LIGHTGRAY; break;
                case 2: color = GRAY; break;
                case 3: color = DARKGRAY; break;
                case -1: color = BLACK; break;
                }
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            }
        }

        // Highlight open and closed lists
        for (Cell* cell : openListDraw) {
            DrawRectangle(cell->x * CELL_SIZE, cell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SKYBLUE);
        }
        for (Cell* cell : closedListDraw) {
            DrawRectangle(cell->x * CELL_SIZE, cell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKBLUE);
        }

        // Draw path so far
        Cell* trace = current;
        while (trace) {
            DrawRectangle(trace->x * CELL_SIZE, trace->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, MAGENTA);
            trace = trace->parent;
        }

        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Slow down for visualization

        if (current == goal) {
            std::vector<Cell*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Check neighbors
        for (auto& dir : directions) {
            int nx = current->x + dir.x;
            int ny = current->y + dir.y;

            if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
                Cell* neighbor = &grid[nx][ny];

                if (!neighbor->isWalkable() || std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end()) {
                    continue;
                }

                float potential_g = current->g + neighbor->weight;

                if (std::find(openList.begin(), openList.end(), neighbor) == openList.end() || potential_g < neighbor->g) {
                    neighbor->g = potential_g;
                    neighbor->h = ManhattanDistance(*neighbor, *goal);
                    neighbor->f = neighbor->g + neighbor->h;
                    neighbor->parent = current;

                    if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                        openList.push_back(neighbor);
                    }
                }
            }
        }
    }

    return {};
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "A* Visualisation");
    SetTargetFPS(60);

    // Initialize grid
    std::vector<std::vector<Cell>> grid(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            grid[x][y] = { x, y, 0, 0, 0, 1, nullptr };
        }
    }

    Cell* player = &grid[0][0];
    std::vector<Cell*> path;
    std::vector<Cell*> openListDraw;
    std::vector<Cell*> closedListDraw;
    int pathIndex = 0;

    while (!WindowShouldClose()) {
        // Handle mouse click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            int mx = mouse.x / CELL_SIZE;
            int my = mouse.y / CELL_SIZE;

            if (mx >= 0 && mx < GRID_SIZE && my >= 0 && my < GRID_SIZE) {
                Cell* target = &grid[mx][my];

                if (target->isWalkable()) {
                    // Reset grid parent pointers for a new pathfinding attempt
                    for (int x = 0; x < GRID_SIZE; ++x) {
                        for (int y = 0; y < GRID_SIZE; ++y) {
                            grid[x][y].parent = nullptr;
                            grid[x][y].g = 0;
                            grid[x][y].f = 0;
                        }
                    }

                    path = FindPath(player, target, grid, openListDraw, closedListDraw);
                    pathIndex = 0; // Reset path index for new path
                }
            }
        }

        // Handle right-click to cycle cell type
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse = GetMousePosition();
            int mx = mouse.x / CELL_SIZE;
            int my = mouse.y / CELL_SIZE;

            if (mx >= 0 && mx < GRID_SIZE && my >= 0 && my < GRID_SIZE) {
                Vector2 currentCell = { mx, my };

                if (!isDragging || currentCell.x != lastModifiedCell.x || currentCell.y != lastModifiedCell.y) {
                    Cell& cell = grid[mx][my];
                    cell.weight = (cell.weight == -1) ? 1 : cell.weight + 1;
                    if (cell.weight > 3) {
                        cell.weight = -1;
                    }

                    lastModifiedCell = currentCell;
                    isDragging = true;
                }
            }
        }
        else {
            isDragging = false;
            lastModifiedCell = { -1, -1 };
        }

        // Move player along the path
        if (!path.empty() && pathIndex < path.size()) {
            player = path[pathIndex];
            pathIndex++;

			// Clear visualisation lists when the path is finished
            if (pathIndex == path.size()) {
                path.clear();
                closedListDraw.clear();
				openListDraw.clear();  
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (openListDraw.size() <= 0) {
			closedListDraw.clear();
        }

        // Draw grid
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                Color color;
                switch (grid[x][y].weight) {
                case 1: color = LIGHTGRAY; break;
                case 2: color = GRAY; break;
                case 3: color = DARKGRAY; break;
                case -1: color = BLACK; break;
                }
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            }
        }

        // Highlight open and closed lists
        for (Cell* cell : openListDraw) {
            DrawRectangle(cell->x * CELL_SIZE, cell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SKYBLUE);
        }
        for (Cell* cell : closedListDraw) {
            DrawRectangle(cell->x * CELL_SIZE, cell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKBLUE);
        }

        // Draw path
        for (Cell* cell : path) {
            DrawRectangle(cell->x * CELL_SIZE, cell->y * CELL_SIZE, CELL_SIZE, CELL_SIZE, MAGENTA);
        }

        // Draw player
        DrawCircle(player->x * CELL_SIZE + CELL_SIZE / 2, player->y * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 4, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


