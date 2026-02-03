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

int main() {
    Graph g(6, false);

    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,3);
    g.addEdge(2,3);
    g.addEdge(3,4);
    g.addEdge(4,5);
    g.addEdge(2,5);

    BFSResult res = bfs(g, 0);

    cout << "BFS order: ";
    for (int v : res.order) cout << v << " ";
    cout << endl;

    return 0;
}

