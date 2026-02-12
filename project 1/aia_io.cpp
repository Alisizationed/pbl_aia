#include "aia_io.h"

#include <cstdio>
#include <string>
#include <vector>

using namespace std;

vector<int> read(string filename) {
    vector<int> a;
    FILE *f = fopen(filename.c_str(), "r");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", filename.c_str());
        return {};
    }

    int x;
    while (fscanf(f, "%d", &x) == 1) {
        a.push_back(x);
    }

    fclose(f);
    return a;
}

void print_to_file(string filename, vector<int> a) {
    if (filename.empty()) return;

    if (filename == "stdout") {
        for (int x: a) printf("%d ", x);
        printf("\n");
        return;
    }

    FILE *f = fopen(filename.c_str(), "w");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", filename.c_str());
        return;
    }
    for (int x: a) fprintf(f, "%d ", x);
    fprintf(f, "\n");
    fclose(f);
}

void run_sort_and_save(
    const vector<int>& nums,
    const string& algorithm_name,
    const string& folder_path,
    const string& file_prefix,
    function<void(vector<int>&)> sort_func
) {
    vector<int> temp = nums;

    long long time_taken = measure([&]() {
        sort_func(temp);
    });

    printf("Size: %ld | %s Sort Time: %lld µs\n", temp.size(), algorithm_name.c_str(), time_taken);

    string output_filename = folder_path + algorithm_name + "/" + file_prefix + ".txt";
    print_to_file(output_filename, temp);
}