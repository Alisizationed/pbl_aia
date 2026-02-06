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
vector<int> read(string filename, int n) {
    // Example of usage of vector
    vector<int> a(n);
    for (int &x: a) scanf("%d", x);
    return a;
}

// TODO: Implement output logic
// TODO: Use fprintf for printing
void print_to_file(string filename, vector<int> a) {
}