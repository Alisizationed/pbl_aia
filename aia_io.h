#ifndef PROJECT_1_AIA_IO_H
#define PROJECT_1_AIA_IO_H
#include <string>
#include <vector>

using namespace std;

template<typename Func>
long long measure(Func f);

vector<int> read(string filename, int n);

void print_to_file(string filename, vector<int> a);

#endif //PROJECT_1_AIA_IO_H