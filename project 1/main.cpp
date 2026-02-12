#pragma GCC optimize("O3,unroll-loops")
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "aia_sort.h"
#include "aia_io.h"

using namespace std;

void check_file(pair<long, string> filename) {
    vector<int> nums = read(filename.second + "input_" + to_string(filename.first) + ".txt");

    run_sort_and_save(nums, "Anastasia", "output_folder/", "output_1000", heap_sort);
}

void analyse() {
    vector<long> input_filenames = {
        100,1000,10000,100000,1000000
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

    int i;

    analyse();

    scanf("%d", &i);

    return 0;
}
