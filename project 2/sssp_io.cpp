#include "sssp_io.h"
#include <fstream>
#include <iostream>

void print_to_file(string &file_name, vector<int>& path, double weight) {
    ofstream out(file_name);

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