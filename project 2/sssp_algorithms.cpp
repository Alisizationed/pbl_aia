#include "sssp_algorithms.h"

double dijkstra_fibonacci(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path) {
    return 0;
}

double dijkstra_binary(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path) {
    return 0;
}

double spfa(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path) {
    int n = adj.size();

    vector<double> dist(n, DBL_MAX);
    vector<int> parent(n, -1);
    vector<bool> in_queue(n, false);

    queue<int> q;

    dist[source] = 0;
    q.push(source);
    in_queue[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;

        for (auto &[v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;

                if (!in_queue[v]) {
                    q.push(v);
                    in_queue[v] = true;
                }
            }
        }
    }

    // reconstruct path
    path.clear();
    if (dist[dest] == DBL_MAX) return -1;

    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    return dist[dest];
}