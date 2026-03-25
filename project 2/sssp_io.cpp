#include "sssp_io.h"

void print_to_file(string &file_name, vector<int>& path, double weight) {
    ofstream fout(file_name);

    if (!fout.is_open()) return;

    if (weight < 0) {
        fout << "No path found\n";
        return;
    }

    fout << "Weight: " << weight << "\n";
    fout << "Path: ";

    for (int node : path) {
        fout << node << " ";
    }

    fout << "\n";
    fout.close();
}