#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
#include "aia_sort.h"
#include "aia_io.h"

using namespace std;
using SortFunc = function<void(vector<int>&)>;

struct Algorithm {
    string name;
    SortFunc func;
};

void check_file(pair<long, string> filename, vector<tuple<long long, long, string>> &results) {
    vector<int> nums = read(filename.second + "input_" + to_string(filename.first) + ".txt");
    vector<Algorithm> algorithms = {
        {"anastasia", heap_sort},
        // {"adelina", bucketSort},
        {"ana", timSort},
        {"andreea", sortArray},
        {"milena", radixSort},
        {"madalina", bubble_sort},
        // {"diana", }
    };

    cout << endl << filename.second << endl;
    for (auto &algo : algorithms) {
        results.push_back(
            make_tuple(
                run_sort_and_save(nums, algo.name, filename.second, "output_" + to_string(filename.first), algo.func),
                filename.first,
                algo.name
            )
        );
    }
}

void analyse() {
    vector<long> input_filenames = {
        100, 1000, 10000, 100000, 1000000
    };
    vector<string> folders = {
        "/tests_ascending/",
        "/tests_descending/",
        "/tests_random/"
    };
    vector<tuple<long long, long, string>> results;
    string main_folder = "sorting_tests/";
    for (long &filesize: input_filenames) {
        for (string &folder : folders) {
            check_file(make_pair(filesize, main_folder + folder), results);
        }
    }
    sort(results.begin(), results.end());
    cout << get<0>(results[0]) << " " << get<0>(results[1]) << " " << get<0>(results[2]) << endl;
}

int main() {
    analyse();

    int i;
    scanf("%d", &i);
    return 0;
}
