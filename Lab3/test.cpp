#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

struct Net {
    string name;
    Point start, end;
    Net(string name, Point start, Point end) : name(name), start(start), end(end) {}
};

struct Cell {
    int cost;
    bool blocked;
    Cell() : cost(1), blocked(false) {}
};

class Grid {
public:
    vector<vector<Cell>> grid;
    vector<Point> directions = {Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0)};

    Grid(int rows, int cols) {
        grid.resize(rows, vector<Cell>(cols));
    }

    bool isValid(int x, int y) {
        return x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size() && !grid[x][y].blocked;
    }

    void addBlock(int x1, int y1, int x2, int y2) {
        for (int i = x1; i <= x2; ++i) {
            for (int j = y1; j <= y2; ++j) {
                grid[i][j].blocked = true;
            }
        }
    }

    void addCost(int x, int y, int cost) {
        if (isValid(x, y)) {
            grid[x][y].cost += cost;
        }
    }
};

struct Node {
    Point point;
    int cost;
    vector<Point> path;

    Node(Point p, int c, vector<Point> pa) : point(p), cost(c), path(pa) {}

    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

vector<Point> AStar(Grid& grid, Point start, Point end) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    vector<vector<bool>> visited(grid.grid.size(), vector<bool>(grid.grid[0].size(), false));
    pq.push(Node(start, 0, {start}));
    visited[start.x][start.y] = true;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.point.x == end.x && current.point.y == end.y) {
            return current.path;
        }

        for (auto dir : grid.directions) {
            int newX = current.point.x + dir.x;
            int newY = current.point.y + dir.y;

            if (grid.isValid(newX, newY) && !visited[newX][newY]) {
                visited[newX][newY] = true;
                vector<Point> newPath = current.path;
                newPath.push_back(Point(newX, newY));
                int newCost = current.cost + grid.grid[newX][newY].cost;
                pq.push(Node(Point(newX, newY), newCost, newPath));
            }
        }
    }

    return {};
}

void ripUpAndReroute(Grid& grid, vector<Net>& nets) {
    for (auto& net : nets) {
        vector<Point> path = AStar(grid, net.start, net.end);
        if (!path.empty()) {
            for (auto& point : path) {
                grid.addCost(point.x, point.y, 1); // Increase cost to discourage future paths
            }
            cout << net.name << " " << path.size() - 1 << endl;
            cout << "begin" << endl;
            for (size_t i = 1; i < path.size(); ++i) {
                cout << path[i-1].x << " " << path[i-1].y << " " << path[i].x << " " << path[i].y << endl;
            }
            cout << "end" << endl;
        } else {
            cout << net.name << " No path found" << endl;
        }
    }
}

int main(int argc, char* argv[]) {
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
        grid.addBlock(x1, y1, x2, y2);
    }

    fin >> netTmp >> netCount;
    vector<Net> nets;
    for (int i = 0; i < netCount; ++i) {
        string name;
        int sx, sy, ex, ey;
        cin >> name >> sx >> sy >> ex >> ey;
        nets.push_back(Net(name, Point(sx, sy), Point(ex, ey)));
    }

    ripUpAndReroute(grid, nets);
    return 0;
}
