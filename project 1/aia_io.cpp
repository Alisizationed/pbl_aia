#include "aia_io.h"

#include "bits/stdc++.h"

using namespace std;

vector<int> read_file(string filename) {
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

void print_to_file(string filename, vector<int> &a) {
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

    string buffer;
    buffer.reserve(a.size() * 12);

    for (int x : a) {
        buffer += to_string(x);
        buffer += ' ';
    }
    buffer += '\n';

    fwrite(buffer.c_str(), 1, buffer.size(), f);
    fclose(f);
}

long long run_sort_and_save(
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

    string output_filename = folder_path + algorithm_name + "/" + file_prefix + ".txt";
    print_to_file(output_filename, temp);

    return time_taken;
}

void standard_sort(vector<int> &nums) {
    sort(nums.begin(), nums.end());
}