#ifndef PROJECT_2_SSSP_ALGORITHMS_H
#define PROJECT_2_SSSP_ALGORITHMS_H
#include <bits/stdc++.h>

using namespace std;

//TODO: Implement Dijkstra Algorithm with Fibonacci Queue for incidence list and return the shortest path and its weight
double dijkstra_fibonacci(vector<tuple<int, int, double>>& list, vector<int>& path);

//TODO: Implement Dijkstra Algorithm with Binary Queue for incidence list and return the shortest path and its weight
double dijkstra_binary(vector<tuple<int, int, double>>& list, vector<int>& path);

//TODO: Implement SPFA Algorithm for incidence list and return the shortest path and its weight
double spfa(vector<tuple<int, int, double>>& list, vector<int>& path);

#endif //PROJECT_2_SSSP_ALGORITHMS_H