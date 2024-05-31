#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

struct Point{
    int x, y;
    Point(int x, int y): x(x), y(y){}
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point& p) const {
        return x != p.x || y != p.y;
    }
};

struct Net {
    Point source;
    Point target;
    string name;
    Net(string n, int sx, int sy, int tx, int ty) : name(n), source(Point(sx, sy)), target(Point(tx, ty)) {}
};

class Grid {
public:
    Grid (int rows, int cols) : rows(rows), cols(cols) {
        grid.resize(rows, vector<int>(cols, 0));
    }
    int getRows() const {
        return rows;
    }
    int getCols() const {
        return cols;
    }
    
    void addBlockage(int x1, int y1, int x2, int y2) {
        for (int i = x1; i <= x2; ++i) {
            for (int j = y1; j <= y2; ++j) {
                grid[i][j] = -1;
            }
        }
    }
    bool isBlocked(int x, int y) const {
        return grid[x][y] == -1;
    }
    bool isWithinBounds(int x, int y) const {
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    bool isPath(int x, int y) const {
        return grid[x][y] == 1;
    }
    void setPath(const vector<Point>& path) {
        for (const auto& p : path) {
            grid[p.x][p.y] = 1;
        }
    }
    void clearPath(const vector<Point>& path) {
        for (const auto& p : path) {
            grid[p.x][p.y] = 0;
        }
    }

    void printGrid() const {
        for (int x = cols - 1; x >= 0; --x) {
            for (int y = 0; y < rows; ++y) {
                cout << (grid[y][x] == -1 ? "#" : (grid[y][x] == 1 ? "+" : " ")) << " ";
            }
            cout << "\n";
        }
    }
private:
    int rows, cols;
    vector<vector<int>> grid;
};

vector<Point> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

bool bfs(Grid& grid, const Point& src, const Point& tgt, vector<Point>& path, const vector<Point>& sources, const vector<Point>& targets) {
    queue<Point> q;
    vector<vector<int>> dist(grid.getRows(), vector<int>(grid.getCols(), INT_MAX));
    vector<vector<Point>> parent(grid.getRows(), vector<Point>(grid.getCols(), Point(-1, -1)));
    
    q.push(src);
    dist[src.x][src.y] = 0;
    
    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        
        if (p.x == tgt.x && p.y == tgt.y) {
            // Backtrack to find the path
            Point cur = tgt;
            while (!(cur.x == src.x && cur.y == src.y)) {
                path.push_back(cur);
                cur = parent[cur.x][cur.y];
            }
            path.push_back(src);
            reverse(path.begin(), path.end());
            return true;
        }
        
        for (auto& dir : directions) {
            int nx = p.x + dir.x, ny = p.y + dir.y;
            if (grid.isWithinBounds(nx, ny) && !grid.isBlocked(nx, ny) && dist[nx][ny] == INT_MAX) {
                dist[nx][ny] = dist[p.x][p.y] + 1;
                parent[nx][ny] = p;
                q.push(Point(nx, ny));
            }
        }
    }
    
    return false;
}

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
    vector<Net> nets;
    // store nets' source and target
    vector<Point> sources, targets;
    for (int i = 0; i < netCount; ++i) {
        string netName;
        int sx, sy, tx, ty;
        fin >> netName >> sx >> sy >> tx >> ty;
        nets.push_back(Net(netName, sx, sy, tx, ty));
    }
    for (const auto& net : nets) {
        sources.push_back(net.source);
        targets.push_back(net.target);
    }
    
    map<string, vector<Point>> routedPaths;

    for (const auto& net : nets) {
        vector<Point> path;
        if (bfs(grid, net.source, net.target, path, sources, targets)) {
            bool intersection = false;
            for (const auto& p : path) {
                if (grid.isPath(p.x, p.y)) {
                    intersection = true;
                    break;
                }
            }

            if (intersection) {
                cout << "intersection" << endl;
                // output net
                cout << net.source.x << " " << net.source.y << " " << net.target.x << " " << net.target.y << endl;
                for (const auto& netToClear : routedPaths) {
                    grid.clearPath(netToClear.second);
                }
                routedPaths.clear();

                for (const auto& netToReroute : nets) {
                    vector<Point> newPath;
                    if (bfs(grid, netToReroute.source, netToReroute.target, newPath, sources, targets)) {
                        grid.setPath(newPath);
                        routedPaths[netToReroute.name] = newPath;
                    } else {
                        fout << "Failed to route " << netToReroute.name << endl;
                        break;
                    }
                }
            } else {
                grid.setPath(path);
                routedPaths[net.name] = path;
            }
        } else {
            fout << "Failed to route " << net.name << endl;
        }
    }

    for (const auto& [netName, path] : routedPaths) {
        fout << netName << " " << path.size() - 2 << endl;
        fout << "begin" << endl;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            fout << path[i].x << " " << path[i].y << " " << path[i + 1].x << " " << path[i + 1].y << endl;
        }
        fout << "end" << endl;
    }

    // grid.printGrid();

    return 0;
}