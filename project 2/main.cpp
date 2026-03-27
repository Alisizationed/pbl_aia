#include <bits/stdc++.h>
#include <iomanip>
#include "sssp_algorithms.h"
#include "sssp_io.h"

using namespace std;
using SsspFunc = function<double(vector<tuple<int, int, double>>& list, vector<int>& path)>;

struct Algorithm {
    string name;
    SsspFunc algorithm;
};

void check_file(const pair<long, string> &filename, vector<tuple<long long, long, string> > &results) {
    vector<string> densities = {"dense", "sparse"};
    vector<Algorithm> algorithms = {
        {"fib_dijkstra", dijkstra_fibonacci},
        {"bin_dijkstra", dijkstra_binary},
        {"spfa", spfa}
    };

    for (const auto &density: densities) {
        string input_file= filename.second + "test_v" + to_string(filename.first) + "_" + density + ".txt";
        vector<tuple<int, int, double> > incidence_list = read_file(input_file);


        for (auto algorithm : algorithms) {
            string output_file = density + "_" + algorithm.name + "_output_" + to_string(filename.first) + "txt";
            long long time_taken = run_sssp_and_save(incidence_list, output_file, algorithm.algorithm);
            results.push_back(make_tuple(time_taken, filename.first, algorithm.name, density));
        }

    }

}

int main() {
    vector<tuple<long long, long, string> > results;
    vector<int> vertex_counts = {100, 1000, 5000, 10000, 50000};
    for (int v : vertex_counts) {
        // pointing to the test_files directory where the inputs are
        check_file(make_pair(v, "./test_files/"), results);
    }

    // print benchmark results to the formatted .txt file
    ofstream out("benchmark_results.txt");

    // set up a clean header with fixed column widths
    out << left << setw(18) << "Algorithm"
        << setw(12) << "Vertices"
        << setw(10) << "Density"
        << "Time(us)\n";
    out << string(55, '-') << "\n"; // dividing line

    // print each result row
    for (const auto& res : results) {
        out << left << setw(18) << get<2>(res)   // algorithm
            << setw(12) << get<1>(res)           // vertices
            << setw(10) << get<3>(res)           // density
            << get<0>(res) << "\n";              // time
    }
    out.close();

    cout << "Benchmarking complete;) Results saved to benchmark_results.txt" << endl;

    cin.get();
    return 0;
}
