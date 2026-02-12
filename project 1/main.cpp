#pragma GCC optimize("O3,unroll-loops")
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "aia_sort.h"
#include "aia_io.h"

using namespace std;
using SortFunc = function<void(vector<int>&)>;

struct Algorithm {
    string name;
    SortFunc func;
};

void check_file(pair<long, string> filename) {
    vector<int> nums = read(filename.second + "input_" + to_string(filename.first) + ".txt");
    vector<Algorithm> algorithms = {
        {"anastasia", heap_sort},
        // {"adelina", bucketSort},
        {"ana", timSort},
        {"andreea", sortArray},
        {"milena", radixSort},
        // {"madalina", },
        // {"diana", }
    };
    vector<tuple<long long, int, string>> results;

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
    string main_folder = "sorting_tests/";
    for (long &filesize: input_filenames) {
        for (string &folder : folders) {
            check_file(make_pair(filesize, main_folder + folder));
        }
    }
}

int main() {
    analyse();

    int i;
    scanf("%d", &i);
    return 0;
}
