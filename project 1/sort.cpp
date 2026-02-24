#pragma GCC optimize("O3,unroll-loops")

#include <bits/stdc++.h>
#include "aia_io.h"
#include "aia_sort.h"
using namespace std;

pair<long long, long long> measure_time(vector<int> &nums, string output_filename) {
    auto start = chrono::high_resolution_clock::now();

    parallel_sort(nums);
    auto end_sort = chrono::high_resolution_clock::now();

    print_to_file(output_filename, nums);
    auto end = chrono::high_resolution_clock::now();

    auto duration_sort = chrono::duration_cast<chrono::microseconds>(end_sort - start);
    auto duration_end = chrono::duration_cast<chrono::microseconds>(end - start);

    return make_pair(duration_sort.count(), duration_end.count());
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./sort <input_file> [output_file|stdout]\n";
        return 1;
    }

    string input_filename = argv[1];
    string output_filename = "";

    if (argc >= 3) {
        output_filename = argv[2];
    }

    vector<int> nums;

    nums = read_file(input_filename);
    pair<long long, long long> time = measure_time(nums, output_filename);
    printf("Time for sort: %lld\n", time.first);
    printf("Time for sort and print: %lld\n", time.second);

    return 0;
}
