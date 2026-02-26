#pragma GCC optimize("O3,unroll-loops")

#include <bits/stdc++.h>
#include "aia_io.h"
#include "aia_sort.h"
using namespace std;

// Function to measure time execution for just sorting and for sorting and printing
pair<long long, long long> measure_time(vector<int> &nums, string output_filename) {
    auto start = chrono::high_resolution_clock::now();

    // Sort the data
    parallel_sort(nums);
    auto end_sort = chrono::high_resolution_clock::now();

    // Print the data
    print_to_file(output_filename, nums);
    auto end = chrono::high_resolution_clock::now();

    // Compute the time ellapsed
    auto duration_sort = chrono::duration_cast<chrono::microseconds>(end_sort - start);
    auto duration_end = chrono::duration_cast<chrono::microseconds>(end - start);

    // Return the time
    return make_pair(duration_sort.count(), duration_end.count());
}

int main(int argc, char *argv[]) {
    // Check if all mandatory argc were passed
    if (argc < 2) {
        cerr << "Usage: ./sort <input_file> [output_file|stdout]\n";
        return 1;
    }

    // Store input_filename and output_filename
    string input_filename = argv[1];
    string output_filename = "";

    // Check if output_filename was passed
    if (argc >= 3) {
        output_filename = argv[2];
    }

    // Declare the vector
    vector<int> nums;

    // Read the input data
    nums = read_file(input_filename);

    // Sort and measure the time ellapsed
    pair<long long, long long> time = measure_time(nums, output_filename);

    // Output the results
    printf("Time for sort: %lld\n", time.first);
    printf("Time for sort and print: %lld\n", time.second);

    return 0;
}
