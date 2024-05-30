#include "Router.h"
#include <queue>
#include <map>
#include <algorithm> // for std::reverse
#include <iostream>  // for std::cerr
#include <functional>
#include <limits> // for std::numeric_limits

Router::Router(Grid &grid) : grid(grid) {}

std::vector<Point> Router::getNeighbors(const Point &p) const {
    static const std::vector<Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<Point> neighbors;
    for (const auto &d : directions) {
        Point np(p.x + d.x, p.y + d.y);
        if (grid.isWithinBounds(np.x, np.y) && !grid.isBlocked(np.x, np.y)) {
            neighbors.push_back(np);
        }
    }
    return neighbors;
}

int Router::heuristic(const Point &a, const Point &b) const {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool Router::route(const Point& source, const Point& target, std::vector<Point>& path, const std::unordered_set<Point, PointHash>& avoidPoints) {
    std::priority_queue<std::pair<int, Point>, std::vector<std::pair<int, Point>>, std::greater<>> openSet;
    std::map<Point, Point> came_from;
    std::map<Point, int> g_score;
    std::map<Point, int> f_score;

    g_score[source] = 0;
    f_score[source] = heuristic(source, target);

    openSet.emplace(f_score[source], source);

    while (!openSet.empty()) {
        Point current = openSet.top().second;
        openSet.pop();

        if (current == target) {
            Point step = current;
            while (step != source) {
                path.push_back(step);
                step = came_from[step];
            }
            path.push_back(source);
            std::reverse(path.begin(), path.end());
            return true;
        }

        for (const auto& next : getNeighbors(current)) {
            if (avoidPoints.find(next) != avoidPoints.end()) continue; // Avoid already used points

            int tentative_g_score = g_score[current] + 1;

            if (g_score.find(next) == g_score.end()) {
                g_score[next] = std::numeric_limits<int>::max();
            }

            if (tentative_g_score < g_score[next]) {
                came_from[next] = current;
                g_score[next] = tentative_g_score;
                f_score[next] = g_score[next] + heuristic(next, target);
                openSet.emplace(f_score[next], next);
            }
        }
    }

    return false; // No path found
}

bool pathIntersects(const std::vector<Point> &path, const std::unordered_set<Point, PointHash> &usedPoints) {
    // std::cout << "Checking for path intersections : " << std::endl;
    for (const auto &p : path) {
        // output p as a path
        // std::cout << "(" << p.x << "," << p.y << ") ";
        if (usedPoints.find(p) != usedPoints.end()) {
            return true;
        }
    }
    return false;
}

bool Router::rerouteSinglePath(std::vector<Point> &path, std::unordered_set<Point, PointHash> &usedPoints)
{
    if (path.empty())   return false;
    Point source = path.front();
    Point target = path.back();
    path.clear();

    // Avoid points used by other paths during rerouting
    std::unordered_set<Point, PointHash> avoidPoints = usedPoints;
    avoidPoints.erase(source); // Ensure the source itself is not avoided

    if (route(source, target, path, avoidPoints))
    {
        for (const auto &p : path)
        {
            usedPoints.insert(p);
        }
        return true;
    }
    return false;
}

bool Router::ripupAndReroute(std::vector<Point>& intersectingPath, std::unordered_set<Point, PointHash>& usedPoints) {
    std::vector<std::vector<Point>> tempPaths;
    std::unordered_set<Point, PointHash> attemptedPoints(intersectingPath.begin(), intersectingPath.end());
    bool rerouted = false;

    std::cout << "Ripup and reroute intersecting path" << std::endl;

    // Rip up all paths that intersect
    for (auto& path : allPaths) {
        if (pathIntersects(path, attemptedPoints)) {
            for (const auto& p : path) {
                usedPoints.erase(p); // Remove used points from the intersecting path
            }
        } else {
            tempPaths.push_back(path);
        }
    }

    allPaths = tempPaths;

    // Re-route the removed paths
    for (auto& path : allPaths) {
        if (!rerouteSinglePath(path, usedPoints)) {
            std::cerr << "Failed to reroute path" << std::endl;
        } else {
            rerouted = true;
        }
    }

    // Iteratively re-route the intersecting path
    while (true) {
        if (!rerouteSinglePath(intersectingPath, usedPoints)) {
            std::cerr << "Failed to reroute intersecting path" << std::endl;
            return false;
        }

        // Check for new intersections
        bool foundIntersection = false;
        for (const auto& path : allPaths) {
            if (pathIntersects(intersectingPath, usedPoints)) {
                foundIntersection = true;
                break;
            }
        }

        if (!foundIntersection) {
            allPaths.push_back(intersectingPath);
            rerouted = true;
            break;
        }
    }

    return rerouted;
}

std::vector<std::vector<Point>> &Router::getAllPaths()
{
    return allPaths;
}
