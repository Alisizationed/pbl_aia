#ifndef PROJECT_2_SSSP_ALGORITHMS_H
#define PROJECT_2_SSSP_ALGORITHMS_H
#include <bits/stdc++.h>

using namespace std;

//TODO: Implement Dijkstra Algorithm for incidence list and return the shortest path and its weight
double dijkstra_with_path(vector<tuple<int, int, double>>& list, vector<int>& path);

#endif //PROJECT_2_SSSP_ALGORITHMS_H