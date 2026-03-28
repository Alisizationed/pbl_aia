# PBL AIA

Projects for PBL AIA.

## Project 2

Single Source Shortest Path (SSSP) Benchmark

This project implements and benchmarks several Single Source Shortest Path (SSSP) algorithms in C++.

### Project Structure

* `main.cpp` – Entry point used to run and measure algorithm performance
* `sssp\\\_io.cpp` – Input/output utility functions
* `sssp\\\_algorithms.cpp` – Implementations of SSSP algorithms

### Implemented Algorithms

```cpp
double dijkstra\\\_fibonacci(vector<vector<pair<int, double>>> \\\&adj, int source, int dest, vector<int>\\\& path);

double dijkstra\\\_binary(vector<vector<pair<int, double>>> \\\&adj, int source, int dest, vector<int>\\\& path);

double spfa(vector<vector<pair<int, double>>> \\\&adj, int source, int dest, vector<int>\\\& path);
```

Each function:

* Takes a graph as an adjacency list (`vector<vector<pair<int, double>>>`)
* Computes the shortest path from `source` to `dest`
* Stores the path in the `path` vector
* Returns the total distance as `double`

### Build Instructions

Run the following commands:

```bash
g++ -std=c++20 -c sssp\\\_io.cpp
g++ -std=c++20 -c sssp\\\_algorithms.cpp

ar rcs libsssp\\\_io.a sssp\\\_io.o
ar rcs libsssp\\\_algs.a sssp\\\_algorithms.o

g++ -std=c++20 main.cpp -L. -lsssp\\\_io -lsssp\\\_algs -o program
```

### Run

```bash
./program.exe
```

(On Linux/macOS use `./program`)

### Notes

* Requires a compiler with C++20 support
* Graph is represented as:

```cpp
  vector<vector<pair<int, double>>>
  ```

* Each edge is stored as `(neighbor, weight)`

### Test Files Format

Test files are structured as follows:

```
v e
a₁ b₁ w₁
a₂ b₂ w₂
...
aₑ bₑ wₑ
```

Where:

* `v` – number of vertices
* `e` – number of edges
* `aᵢ` – starting vertex of edge *i*
* `bᵢ` – ending vertex of edge *i*
* `wᵢ` – weight of edge *i*

### Purpose

This project is intended for:

* Comparing performance of SSSP algorithms
* Studying differences between heap implementations
* Educational use

### Possible Improvements

* Include more algorithms (e.g., Bellman-Ford)
* Create new libraries for data structures used

