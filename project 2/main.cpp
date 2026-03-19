#include <bits/stdc++.h>
#include "sssp_algorithms.h"
#include "sssp_io.h"

using namespace std;

void check_file(const pair<long, string> &filename, vector<tuple<long long, long, string> > &results) {

    vector<string> densities = {"dense/", "sparse/"};

    for (const auto &density: densities) {
        vector<tuple<int, int, double> > incidence_list = read_file(
            filename.second + density + "input_" + to_string(filename.first) + ".txt");

        // TODO: Make it to run for different algorithms
        string folder_name = "dijkstra/";

        string file_name = density + folder_name + "/output_" + to_string(filename.first) + "txt";
        long long time_taken = run_sssp_and_save(incidence_list, file_name, dijkstra_with_path);
        results.push_back(make_tuple(time_taken, filename.first, folder_name));
    }

    // TODO: Print the results of algorithms
}

int main() {
    vector<tuple<long long, long, string> > results;
    for (int i = 100; i <= 1e5; i *= 10) {
        check_file(make_pair(i, "./tests/"), results);
    }
    cin.get();
    return 0;
}
