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

