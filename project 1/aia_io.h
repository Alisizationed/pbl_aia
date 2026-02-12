#ifndef PROJECT_1_AIA_IO_H
#define PROJECT_1_AIA_IO_H
#include <chrono>
#include <string>
#include <vector>
#include <functional>

using namespace std;

template<typename Func>
long long measure(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    return duration.count();
}

vector<int> read(string filename);

void print_to_file(string filename, vector<int> a);

void run_sort_and_save(
    const vector<int> &nums,
    const string &algorithm_name,
    const string &folder_path,
    const string &file_prefix,
    function<void(vector<int> &)> sort_func
);

#endif //PROJECT_1_AIA_IO_H
