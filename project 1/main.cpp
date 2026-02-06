#pragma GCC optimize("O3,unroll-loops")
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "aia_sort.h"
#include "aia_io.h"

using namespace std;

void check_file(pair<long, string> filename) {
    vector<int> nums = read(filename.second, filename.first);

    // TODO: Add benchmarking for each sorting algorithm
    vector<int> nums_adelina = nums;
    long long time_adelina = measure([&]() {
        sort_adelina(nums_adelina);
    });

    // TODO: Enhance printing
    printf("%lld\n", time_adelina);

    // TODO: Read output filename
    string output_filename = "output.txt";
    print_to_file(output_filename, nums_adelina);
}

// TODO: Implement the main logic
void analyse() {
    // TODO: Add testfiles' filenames
    vector<pair<long, string> > input_filenames = {};

    for (pair<long, string> &filename: input_filenames) {
        check_file(filename);
    }
}

int main() {
    analyse();
    return 0;
}
