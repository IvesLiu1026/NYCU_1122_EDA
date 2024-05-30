#include "Grid.h"
#include "Router.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
using namespace std;

void addPathToUsedPoints(const vector<Point> &path, unordered_set<Point, PointHash> &usedPoints);
void outputPath(const vector<Point>& path, ofstream& fout);


int main(int argc, char *argv[]) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    string rowtmp, coltmp, blockTmp, netTmp;
    int rows, cols, blockCount, netCount;
    fin >> rowtmp >> rows >> coltmp >> cols;

    Grid grid(rows, cols);

    fin >> blockTmp >> blockCount;
    for (int i = 0; i < blockCount; ++i) {
        int x1, y1, x2, y2;
        fin >> x1 >> x2 >> y1 >> y2;
        grid.addBlockage(x1, y1, x2, y2);
    }

    fin >> netTmp >> netCount;
    Router router(grid);
    unordered_set<Point, PointHash> usedPoints;

    for (int i = 0; i < netCount; ++i) {
        string netName;
        int sx, sy, tx, ty;
        fin >> netName >> sx >> sy >> tx >> ty;

        Point source(sx, sy);
        Point target(tx, ty);
        // router.targetPoints.insert(target);
        vector<Point> path;
        if (router.route(source, target, path, usedPoints)) {
            while (pathIntersects(path, usedPoints)) {
                if (router.ripupAndReroute(path, usedPoints)) {
                    cerr << "Failed to reroute paths to avoid intersection for " << netName << endl;
                    break;
                }
            }
            if (!path.empty()) {
                // Access allPaths through the non-const getter
                auto& allPaths = router.getAllPaths();
                allPaths.push_back(path);
                addPathToUsedPoints(path, usedPoints);
                grid.setPath(path);
                
                fout << netName << " " << path.size() - 2  << endl;
                fout << "begin" << endl;
                outputPath(path, fout);
                fout << "end" << endl;
            }
        } else {
            cerr << "No path found for " << netName << endl;
        }
    }

    // grid.printGrid();

    return 0;
}

void addPathToUsedPoints(const vector<Point> &path, unordered_set<Point, PointHash> &usedPoints) {
    for (const auto &p : path) {
        usedPoints.insert(p); 
    }
}
void outputPath(const vector<Point>& path, ofstream& fout) {
    if (path.empty()) return;

    fout << path.front().x << " " << path.front().y << " ";
    Point currentDirection = { path[1].x - path[0].x, path[1].y - path[0].y };

    for (size_t i = 1; i < path.size() - 1; ++i) {
        Point nextDirection = { path[i + 1].x - path[i].x, path[i + 1].y - path[i].y };
        if (currentDirection.x != nextDirection.x || currentDirection.y != nextDirection.y) {
            fout << path[i].x << " " << path[i].y << endl;
            fout << path[i].x << " " << path[i].y << " ";
            currentDirection = nextDirection;
        }
    }

    fout << path.back().x << " " << path.back().y << endl;
}