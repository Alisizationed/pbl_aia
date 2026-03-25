#include <bits/stdc++.h>
#include "sssp_algorithms.h"
#include "sssp_io.h"

using namespace std;
using SsspFunc = function<double(vector<tuple<int, int, double>>& list, vector<int>& path)>;

struct Algorithm {
    string name;
    SsspFunc algorithm;
};

void check_file(const pair<long, string> &filename, vector<tuple<long long, long, string> > &results) {
    vector<string> densities = {"dense/", "sparse/"};
    vector<Algorithm> algorithms = {
        {"fib_dijkstra", dijkstra_fibonacci},
        {"bin_dijkstra", dijkstra_binary},
        {"spfa", spfa}
    };

    for (const auto &density: densities) {
        vector<tuple<int, int, double> > incidence_list = read_file(
            filename.second + density + "input_" + to_string(filename.first) + ".txt");

        for (auto algorithm : algorithms) {
            string file_name = density + algorithm.name + "/output_" + to_string(filename.first) + "txt";
            long long time_taken = run_sssp_and_save(incidence_list, file_name, algorithm.algorithm);
            results.push_back(make_tuple(time_taken, filename.first, algorithm.name));
        }

    }

    // TODO: Print the results of algorithms
    cout << "\nResults for size " << filename.first << "\n";

    for (const auto& [time, size, algo] : results) {
        if (size == filename.first) {
            cout << algo << " -> " << time << " microseconds\n";
        }
    }
}

int main() {
    vector<tuple<long long, long, string> > results;
    for (int i = 100; i <= 1e5; i *= 10) {
        check_file(make_pair(i, "./tests/"), results);
    }
    cin.get();
    return 0;
}
