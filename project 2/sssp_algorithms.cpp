#include "sssp_algorithms.h"

double dijkstra_fibonacci(vector<tuple<int, int, double>>& list, vector<int>& path) {

    // Find how many nodes are in the graph
    int n = 0;
    for (auto& [src, dst, w] : list) {
        n = max(n, max(src, dst) + 1);
    }

    // Build adjacency list: neighbors[u] = list of (neighbor, weight)
    vector<vector<pair<int, double>>> neighbors(n);
    for (auto& [src, dst, w] : list) {
        neighbors[src].push_back({dst, w});
    }

    // Set all distances to infinity, source = 0
    const double INF = numeric_limits<double>::infinity();
    vector<double> dist(n, INF);
    vector<int> prev(n, -1);
    dist[0] = 0.0;

    // Our deque-based queue, start with the source node
    deque<int> queue;
    queue.push_back(0);

    while (!queue.empty()) {

        // LLL: if the front node is too expensive, send it to the back
        double total = 0.0;
        for (int node : queue) total += dist[node];
        double average = total / queue.size();

        while (dist[queue.front()] > average) {
            queue.push_back(queue.front());
            queue.pop_front();

            total = 0.0;
            for (int node : queue) total += dist[node];
            average = total / queue.size();
        }

        // Take the front node and process it
        int current = queue.front();
        queue.pop_front();

        // Check every neighbor of current node
        for (auto& [next, weight] : adj[current]) {

            double new_dist = dist[current] + weight;

            if (new_dist < dist[next]) {
                dist[next] = new_dist;
                prev[next] = current;

                // SLF: if next is cheaper than the front, put it at the front
                if (!queue.empty() && dist[next] < dist[queue.front()]) {
                    queue.push_front(next);
                } else {
                    queue.push_back(next);
                }
            }
        }
    }

    // Reconstruct path by walking backwards from destination to source
    int destination = n - 1;

    if (dist[destination] == INF) {
        path.clear();
        return INF;
    }

    for (int node = destination; node != -1; node = prev[node]) {
        path.push_back(node);
    }
    reverse(path.begin(), path.end());

    return dist[destination];
}


