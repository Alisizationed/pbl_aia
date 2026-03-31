#include "sssp_io.h"
#include <fstream>
#include <iostream>

vector<vector<pair<int, double>>> read_file(string file_name, int n) {
    ifstream ifs(file_name);

    if (!ifs.is_open()) {
        cerr << "Cannot open file: " << file_name << endl;
        exit(-1);
    }

    int m;
    ifs >> m;
    ifs >> m;

    vector<vector<pair<int, double>>> adj(n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        double w;
        ifs >> u >> v >> w;
        adj[u].emplace_back(make_pair(v, w));
    }

    ifs.close();

    return adj;
}

void print_to_file(string &file_name, vector<int>& path, double weight) {
    ofstream out("./results/" + file_name);

    if (!out.is_open()) {
        cerr << "Error opening file: " << file_name << endl;
        return;
    }

    out << "Shortest Path Weight: " << weight << "\n";
    out << "Path: ";

    if (path.empty()) {
        out << "No path found.\n";
    } else {
        // print path with arrows, stopping before the last element to avoid a trailing arrow
        for (size_t i = 0; i < path.size(); ++i) {
            out << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        out << "\n";
    }

    out.close();
}