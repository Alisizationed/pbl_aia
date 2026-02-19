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
        // {"adelina", countSort},
        {"ana", timSort},
        {"andreea", sortArray},
        {"milena", radixSort},
        {"madalina", bubble_sort},
        {"diana", shell_sort},
        {"prebuilt", standard_sort},
    };

    cout << endl << filename.second << endl;
    for (auto &algo : algorithms) {
        if ((algo.name == "andreea" || algo.name == "milena" || algo.name == "madalina") && nums.size() >= 1e5)
            continue;
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
        "tests_ascending/",
        "tests_descending/",
        "tests_random/"
    };
    vector<vector<tuple<long long, long, string>>> results(input_filenames.size() * folders.size());
    string main_folder = "sorting_tests/";
    int i = 0;
    for (long &filesize: input_filenames) {
        for (string &folder : folders) {
            check_file(make_pair(filesize, main_folder + folder), results[i]);
            sort(results[i].begin(), results[i].end());
            tuple<long long, long, string> &best = results[i][0];
            cout << "The best: "
                 << get<2>(best) << " "
                 << get<0>(best) << "us\n";
            i++;
        }
    }
}

int main() {
    analyse();

    int i;
    scanf("%d", &i);
    return 0;
}
