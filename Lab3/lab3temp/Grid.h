#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class Grid {
    
public:
    Grid(int rows, int cols);
    void addBlockage(int x1, int y1, int x2, int y2);
    bool isBlocked(int x, int y) const;
    bool isWithinBounds(int x, int y) const;
    void setPath(const std::vector<Point>& path);
    // void printGrid() const;

private:
    int rows, cols;
    std::vector<std::vector<int>> grid;
};

#endif  // GRID_H
