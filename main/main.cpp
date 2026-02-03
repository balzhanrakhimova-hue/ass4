#include <bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    bool directed;
    vector<vector<int>> adj;

public:
    Graph(int V, bool directed) : V(V), directed(directed) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        if (!directed) adj[v].push_back(u);
    }

    vector<int> neighbors(int u) {
        return adj[u];
    }

    int vertices() {
        return V;
    }
};

struct BFSResult {
    vector<int> order;
    vector<int> dist;
    vector<int> parent;
};

BFSResult bfs(Graph& g, int start) {
    int V = g.vertices();
    BFSResult res;
    res.dist.assign(V, -1);
    res.parent.assign(V, -1);

    queue<int> q;
    q.push(start);
    res.dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.order.push_back(u);

        for (int v : g.neighbors(u)) {
            if (res.dist[v] == -1) {
                res.dist[v] = res.dist[u] + 1;
                res.parent[v] = u;
                q.push(v);
            }
        }
    }
    return res;
}
vector<int> shortestPathUnweighted(Graph& g, int s, int t) {
    BFSResult res = bfs(g, s);

    if (res.dist[t] == -1) {
        return {}; 
    }

    vector<int> path;
    for (int v = t; v != -1; v = res.parent[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());
    return path;
}
void dfsRecursiveUtil(Graph& g, int v, vector<bool>& visited, vector<int>& order) {
    visited[v] = true;
    order.push_back(v);

    for (int u : g.neighbors(v)) {
        if (!visited[u]) {
            dfsRecursiveUtil(g, u, visited, order);
        }
    }
}

vector<int> dfsRecursive(Graph& g, int start) {
    vector<bool> visited(g.vertices(), false);
    vector<int> order;

    dfsRecursiveUtil(g, start, visited, order);
    return order;
}
vector<int> dfsIterative(Graph& g, int start) {
    vector<bool> visited(g.vertices(), false);
    vector<int> order;
    stack<int> st;

    st.push(start);

    while (!st.empty()) {
        int v = st.top();
        st.pop();

        if (visited[v]) continue;

        visited[v] = true;
        order.push_back(v);

        // Чтобы порядок был похож на рекурсивный DFS,
        // кладём соседей в стек в обратном порядке
        vector<int> neigh = g.neighbors(v);
        reverse(neigh.begin(), neigh.end());

        for (int u : neigh) {
            if (!visited[u]) {
                st.push(u);
            }
        }
    }
    return order;
}
bool dfsCycleUtil(Graph& g, int v, vector<int>& color) {
    color[v] = 1; // GRAY

    for (int u : g.neighbors(v)) {
        if (color[u] == 1) {
            return true; // найден цикл
        }
        if (color[u] == 0) {
            if (dfsCycleUtil(g, u, color))
                return true;
        }
    }

    color[v] = 2; // BLACK
    return false;
}

bool hasCycleDirected(Graph& g) {
    vector<int> color(g.vertices(), 0); // 0 WHITE

    for (int i = 0; i < g.vertices(); i++) {
        if (color[i] == 0) {
            if (dfsCycleUtil(g, i, color))
                return true;
        }
    }
    return false;
}
struct Cell {
    int r, c;
};

vector<Cell> mazeShortestPath(vector<string>& grid, Cell S, Cell T) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> dist(n, vector<int>(m, -1));
    vector<vector<Cell>> parent(n, vector<Cell>(m, {-1, -1}));

    queue<Cell> q;
    q.push(S);
    dist[S.r][S.c] = 0;

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!q.empty()) {
        Cell cur = q.front(); q.pop();

        if (cur.r == T.r && cur.c == T.c)
            break;

        for (int i = 0; i < 4; i++) {
            int nr = cur.r + dr[i];
            int nc = cur.c + dc[i];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m)
                continue;
            if (grid[nr][nc] == '#')
                continue;
            if (dist[nr][nc] != -1)
                continue;

            dist[nr][nc] = dist[cur.r][cur.c] + 1;
            parent[nr][nc] = cur;
            q.push({nr, nc});
        }
    }

    if (dist[T.r][T.c] == -1)
        return {}; // пути нет

    vector<Cell> path;
    for (Cell cur = T; cur.r != -1; cur = parent[cur.r][cur.c]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}


int main() {
    cout << "===== GRAPH DEMO =====\n";

    // --- Graph + BFS + Shortest Path ---
    Graph g(6, false);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,3);
    g.addEdge(2,3);
    g.addEdge(3,4);
    g.addEdge(4,5);
    g.addEdge(2,5);

    BFSResult bfsRes = bfs(g, 0);
    cout << "BFS order: ";
    for (int v : bfsRes.order) cout << v << " ";
    cout << "\n";

    vector<int> path = shortestPathUnweighted(g, 0, 5);
    cout << "Shortest path 0 -> 5: ";
    for (int v : path) cout << v << " ";
    cout << "\n\n";

    // --- DFS ---
    vector<int> dfsRec = dfsRecursive(g, 0);
    vector<int> dfsIt  = dfsIterative(g, 0);

    cout << "DFS recursive: ";
    for (int v : dfsRec) cout << v << " ";
    cout << "\n";

    cout << "DFS iterative: ";
    for (int v : dfsIt) cout << v << " ";
    cout << "\n\n";

    // --- Cycle detection (directed) ---
    Graph dg(4, true);
    dg.addEdge(0,1);
    dg.addEdge(1,2);
    dg.addEdge(2,3);
    dg.addEdge(3,1);

    cout << "Directed graph cycle: ";
    cout << (hasCycleDirected(dg) ? "YES" : "NO") << "\n\n";

    // --- Maze (Applied problem) ---
    cout << "===== MAZE SHORTEST PATH =====\n";

    vector<string> grid = {
        "S..#",
        ".#..",
        ".#T.",
        "...."
    };

    Cell S, T;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'S') S = {i, j};
            if (grid[i][j] == 'T') T = {i, j};
        }
    }

    vector<Cell> mazePath = mazeShortestPath(grid, S, T);

    if (mazePath.empty()) {
        cout << "No path found in maze\n";
    } else {
        cout << "Maze shortest path length: " << mazePath.size() - 1 << "\n";
        cout << "Path: ";
        for (auto c : mazePath) {
            cout << "(" << c.r << "," << c.c << ") ";
        }
        cout << "\n";
    }

    return 0;
}
