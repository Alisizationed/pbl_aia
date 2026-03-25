#include "sssp_algorithms.h"

double spfa(vector<tuple<int, int, double>>& list, vector<int>& path) {
    // number of nodes
    int n = 0;
    for (auto &[u, v, w] : list) {
        n = max(n, max(u, v));
    }
    n++;

    // adjacency list
    vector<vector<pair<int, double>>> adj(n);
    for (auto &[u, v, w] : list) {
        adj[u].push_back({v, w});
    }

    vector<double> dist(n, DBL_MAX);
    vector<int> parent(n, -1);
    vector<bool> in_queue(n, false);

    queue<int> q;

    int source = 0;
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

    // destination = last node
    int dest = n - 1;

    // reconstruct path
    path.clear();
    if (dist[dest] == DBL_MAX) return -1;

    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    return dist[dest];
}