#include "aia_io.h"

#include <cstdio>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// TODO: Implement Time Checker function
template<typename Func>
long long measure(Func f){
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    return duration.count();
}

// TODO: Implement reading from a file
// TODO: Use fscanf for reading
vector<int> read(string filename, int n) {
    vector<int> a;
    a.reserve(n);

    FILE* f = fopen(filename.c_str(), "r");
    if (!f) {
        printf("Cannot open file %s\n", filename.c_str());
        return a;
    }

    int x;
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d", &x) != 1) break;
        a.push_back(x);
    }

    fclose(f);
    return a;
}

// TODO: Implement output logic
// TODO: Use fprintf for printing
void print_to_file(string filename, vector<int> a) {
    FILE* f = fopen(filename.c_str(), "w");
    if (!f) {
        printf("Cannot open file %s\n", filename.c_str());
        return;
    }

    for (int x : a) {
        fprintf(f, "%d ", x);
    }

    fclose(f);
}