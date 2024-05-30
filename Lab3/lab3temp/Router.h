#ifndef ROUTER_H
#define ROUTER_H

#include "Grid.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>

struct PointHash {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
};

class Router {
public:
    Router(Grid& grid);
    bool route(const Point& source, const Point& target, std::vector<Point>& path, const std::unordered_set<Point, PointHash>& avoidPoints);
    bool ripupAndReroute(std::vector<Point>& path, std::unordered_set<Point, PointHash>& usedPoints);    
    std::vector<std::vector<Point>>& getAllPaths();
    bool rerouteSinglePath(std::vector<Point>& path, std::unordered_set<Point, PointHash>& usedPoints);

private:
    Grid& grid;
    std::vector<std::vector<Point>> allPaths;
    std::vector<Point> getNeighbors(const Point& p) const;
    int heuristic(const Point& a, const Point& b) const;
};

bool pathIntersects(const std::vector<Point>& path, const std::unordered_set<Point, PointHash>& usedPoints);
#endif // ROUTER_H
