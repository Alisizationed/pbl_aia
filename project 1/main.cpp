#pragma GCC optimize("O3,unroll-loops")
#include "bits/stdc++.h"
#include "aia_sort.h"
#include "aia_io.h"
#include <iomanip>

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

using namespace std;
using SortFunc = function<void(vector<int>&)>;

struct Algorithm {
    string name;
    SortFunc func;
};

void check_file(pair<long, string> filename, vector<tuple<long long, long, string>> &results) {
    vector<int> nums = read_file(filename.second + "input_" + to_string(filename.first) + ".txt");

    vector<Algorithm> algorithms = {
        {"Anastasia (heap sort)", heap_sort},
        {"Adelina (intro sort)", introsort},
        {"Ana (tim sort)", timSort},
        {"Andreea (quick sort)", sort_andreea},
        {"Milena (radix sort)", radixSort},
        {"Madalina (bubble sort)", bubble_sort},
        {"Diana (shell sort)", shell_sort},
        {"Parallel", parallel_sort},
        {"Prebuilt", standard_sort},
    };

    cout << CYAN << BOLD << "\n=================================================================\n" << RESET;
    cout << BOLD << " 📁 Folder: " << YELLOW << filename.second
         << RESET << BOLD << " | 📊 Size: " << YELLOW << filename.first << RESET << "\n";
    cout << CYAN << BOLD << "-----------------------------------------------------------------\n" << RESET;

    cout << left << setw(28) << BOLD << "Algorithm"
         << setw(20) << "Time (us)"
         << "Status" << RESET << "\n";
    cout << CYAN << "-----------------------------------------------------------------\n" << RESET;

    for (auto &algo : algorithms) {
        if ((algo.name == "Milena (radix sort)" || algo.name == "Madalina (bubble sort)") && nums.size() >= 1e5) {
            cout << left << setw(28) << algo.name
                 << setw(20) << "N/A"
                 << RED << "Skipped (Size >= 10^5)" << RESET << "\n";
            continue;
        }

        // Extract everything before the first space
        string folder_name = algo.name.substr(0, algo.name.find(' '));

        // Convert the very first letter back to lowercase
        folder_name[0] = tolower(folder_name[0]);

        // Pass the newly lowercased folder_name to save the file
        long long time_taken = run_sort_and_save(nums, folder_name, filename.second, "output_" + to_string(filename.first), algo.func);
        results.push_back(make_tuple(time_taken, filename.first, algo.name));

        // Print the result row to the console
        cout << left << setw(28) << algo.name
             << setw(20) << time_taken
             << GREEN << "✅ Done" << RESET << "\n";
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

    cout << BOLD << GREEN << "\n🚀 STARTING SORTING ALGORITHM BENCHMARKS 🚀\n" << RESET;

    for (long &filesize: input_filenames) {
        for (string &folder : folders) {
            check_file(make_pair(filesize, main_folder + folder), results[i]);
            sort(results[i].begin(), results[i].end());

            // Highlight the winner at the bottom of the table
            tuple<long long, long, string> &best = results[i][0];
            cout << CYAN << "------------------------------------------------------\n" << RESET;
            cout << BOLD << "🏆 Best Algorithm: " << GREEN << get<2>(best) << RESET
                 << BOLD << " with " << GREEN << get<0>(best) << " us" << RESET << "\n";
            cout << CYAN << BOLD << "======================================================\n" << RESET;
            i++;
        }
    }

    cout << BOLD << GREEN << "\n✅ BENCHMARKS COMPLETED!\n\n" << RESET;
}

int main() {
    analyse();

    cout << BOLD << "Press Enter to exit..." << RESET;
    cin.ignore();
    cin.get();
    return 0;
}