#include "aia_io.h"

#include <cstdio>
#include <string>
#include <vector>

using namespace std;

// TODO: Implement Time Checker function
template<typename Func>
long long measure(Func f);

// TODO: Implement reading from a file
// TODO: Use fscanf for reading
vector<int> read(string filename, int n = 0) {
    vector<int> a;
    FILE* f = fopen (filename.c_str(), "r");
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

// TODO: Implement output logic
// TODO: Use fprintf for printing
void print_to_file(string filename, const vector<int>& a) {
    if (filename.empty()) return;

    if (filename == "stdout") {
        for (int x : a) printf("%d ", x);
        printf("\n");
        return;
    }

    FILE* f = fopen(filename.c_str(), "w");
    if (!f) {
        fprintf(stderr, "Error opening file %s\n", filename.c_str());
        return;
    }
    for (int x : a) fprintf(f, "%d ", x);
    fprintf(f, "\n");
    fclose(f);
}