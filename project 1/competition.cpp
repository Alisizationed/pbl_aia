#include <bits/stdc++.h>
#include "aia_io.h"
#include "aia_sort.h"
using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: ./sort <input_file> [output_file|stdout]\n";
        return 1;
    }

    string input_filename = argv[1];
    string output_filename = "";

    if (argc >= 3) {
        output_filename = argv[2];
    }

    // Open input file
    ifstream input(input_filename);
    if (!input) {
        cerr << "Error opening input file.\n";
        return 1;
    }

    // Decide output destination
    if (output_filename == "" || output_filename == "stdout") {
        // Print to console
        cout << "Writing to stdout\n";
    } else {
        ofstream output(output_filename);
        if (!output) {
            cerr << "Error opening output file.\n";
            return 1;
        }
        cout << "Writing to file: " << output_filename << "\n";
    }

    vector<int> nums;

    nums = read_file(input_filename);
    parallel_sort(nums);
    print_to_file(output_filename, nums);

    return 0;
}