#ifndef PROJECT_2_SSSP_ALGORITHMS_H
#define PROJECT_2_SSSP_ALGORITHMS_H
#include <bits/stdc++.h>

using namespace std;

//TODO: Implement Dijkstra Algorithm with Fibonacci Queue for incidence list and return the shortest path and its weight
double dijkstra_fibonacci(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);

//TODO: Implement Dijkstra Algorithm with Binary Queue for incidence list and return the shortest path and its weight
double dijkstra_binary(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);

double spfa(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);

#endif //PROJECT_2_SSSP_ALGORITHMS_H