#ifndef PROJECT_2_SSSP_IO_H
#define PROJECT_2_SSSP_IO_H
#include <bits/stdc++.h>

using namespace std;

template<typename Func>
long long measure(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    return duration.count();
}

vector<vector<pair<int, double>>> read_file(string file_name, int n);

void print_to_file(string &file_name, vector<int>& path, double weight);

inline long long run_sssp_and_save(
    vector<vector<pair<int, double>>> & adj,
    int source,
    int dest,
    string file_name,
    const function<double(vector<vector<pair<int, double>>>&, int source, int dest, vector<int>&)> &algorithm
) {
    vector<int> path;
    double weight;

    long long time_taken = measure([&]() {
        weight = algorithm(adj, source, dest, path);
    });
    print_to_file(file_name, path, weight);

    return time_taken;
}

#endif //PROJECT_2_SSSP_IO_H