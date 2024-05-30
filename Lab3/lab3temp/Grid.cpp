#include "Grid.h"
#include <iostream>

Grid::Grid(int rows, int cols) : rows(rows), cols(cols) {
    grid.resize(rows, std::vector<int>(cols, 0));
}

void Grid::addBlockage(int x1, int y1, int x2, int y2) {
    for (int i = x1; i <= x2; ++i) {
        for (int j = y1; j <= y2; ++j) {
            grid[i][j] = -1;
        }
    }
}

bool Grid::isBlocked(int x, int y) const {
    return grid[x][y] == -1;
}

bool Grid::isWithinBounds(int x, int y) const {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

void Grid::setPath(const std::vector<Point>& path) {
    for (const auto& p : path) {
        grid[p.x][p.y] = 1;
    }
}

// void Grid::printGrid() const {
//     for (int x = cols - 1; x >= 0; --x) {
//         for (int y = 0; y < rows; ++y) {
//             std::cout << (grid[y][x] == -1 ? "#" : (grid[y][x] == 1 ? "+" : " ")) << " ";
//         }
//         std::cout << "\n";
//     }
// }