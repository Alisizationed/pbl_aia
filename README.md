# PBL AIA

Projects for PBL AIA.

## Project 3

To run this project, first clone the repo:

```
git clone https://github.com/Alisizationed/pbl_aia
```

Then, built the docker container for backend by running in folder /backend:

```
docker build -t backend
```

To run the ```docker-compose.yaml```, in /pbl_aia``` run in cmd:

```
docker-compose up
```

Next step is importing the realm to keycloak:
open ```http:localhost:8080```, click on "Import realm" and attach there the json file from ```/keycloak```.

Get into ```users``` and create a new user to be used for work. To give the privileges, give the realm roles to the user.

Then

```
docker exec -i railway-postgres psql -U railway_user -d railway_db < schema.sql
docker exec -i railway-postgres psql -U railway_user -d railway_db < update.sql
docker exec -i railway-postgres psql -U railway_user -d railway_db < fix.sql
```

You're ready to work!

## Project 2

Single Source Shortest Path (SSSP) Benchmark

This project implements and benchmarks several Single Source Shortest Path (SSSP) algorithms in C++.

### Project Structure

* `main.cpp` – Entry point used to run and measure algorithm performance
* `sssp_io.cpp` – Input/output utility functions
* `sssp_algorithms.cpp` – Implementations of SSSP algorithms

### Implemented Algorithms

```cpp
double dijkstra_fibonacci(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);

double dijkstra_binary(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);

double spfa(vector<vector<pair<int, double>>> &adj, int source, int dest, vector<int>& path);
```

Each function:

* Takes a graph as an adjacency list (`vector<vector<pair<int, double>>>`)
* Computes the shortest path from `source` to `dest`
* Stores the path in the `path` vector
* Returns the total distance as `double`

### Build Instructions

Run the following commands:

```bash
g++ -std=c++20 -c sssp_io.cpp
g++ -std=c++20 -c sssp_algorithms.cpp

ar rcs libsssp_io.a sssp_io.o
ar rcs libsssp_algs.a sssp_algorithms.o

g++ -std=c++20 main.cpp -L. -lsssp_io -lsssp_algs -o program
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

