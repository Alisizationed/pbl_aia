#include "sssp_algorithms.h"
#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <functional>
#include <cmath>
using namespace std;

double dijkstra_fibonacci(vector<tuple<int, int, double>>& list, vector<int>& path) {

    // Find how many nodes are in the graph
    int n = 0;
    for (auto& [src, dst, w] : list) {
        n = max(n, max(src, dst) + 1);
    }

    // Build adjacency list: neighbors[u] = list of (neighbor, weight)
    vector<vector<pair<int, double>>> neighbors(n);
    for (auto& [src, dst, w] : list) {
        neighbors[src].push_back({dst, w});
    }

    // Set all distances to infinity, source = 0
    const double INF = numeric_limits<double>::infinity();
    vector<double> dist(n, INF);
    vector<int> prev(n, -1);
    dist[0] = 0.0;

    //Fibonacci heap node
    struct Node {
        int vertex;
        double key;
        int degree;
        bool marked;
        Node *parent, *child, *prev, *next;
        Node(int v, double k) : vertex(v), key(k), degree(0), marked(false), parent(nullptr), child(nullptr), prev(this), next(this) {}
    };

    //Heap state
    Node* min_node=nullptr;
    int heap_size=0;
    vector<Node*> node_ptr(n);

    // Splice x into root list
    auto splice = [&](Node* x) {
        if (!min_node) {
            x->prev=x->next=x;
            min_node=x;
        } else {
            x->next=min_node;
            x->prev=min_node->prev;
            min_node->prev->next=x;
            min_node->prev=x;
            if (x->key < min_node->key) min_node=x;
        }
    };

    //Link y as child of x
    auto link=[&](Node* y, Node* x) {
        y->prev->next=y->next;
        y->next->prev=y->prev;
        y->parent=x;
        if (!x->child) {
            x->child=y;
            y->prev=y->next=y;
        } else {
            y->next=x->child;
            y->prev=x->child->prev;
            x->child->prev->next=y;
            x->child->prev=y;
        }
        x->degree++;
        y->marked=false;
    };

    //Consolidate root list
    auto consolidate=[&]() {
        int max_degree=(int)log2(heap_size +1)+2;
        vector<Node*> table(max_degree+1,nullptr);

        vector<Node*> roots;
        Node* node=min_node;
        do{ roots.push_back(node); node=node->next; } while(node!=min_node);

        for (Node* w: roots) {
            Node* x=w;
            int d=x->degree;
            while (d<=max_degree && table[d]) {
                Node* y=table[d];
                if (x->key > y->key) swap(x,y);
                link(y,x);
                table[d++]=nullptr;
            }
            table[min(d,max_degree)]=x;
        }
        min_node=nullptr;
        for (Node* nd: table) {
            if (nd) { nd->prev=nd->next=nd; splice(nd);}
        }
    };
    //Cut x from parent y, move to root list
    function<void(Node*, Node*)> node=[&](Node* x, Node* y) {
        if (x->next==x) y->child=nullptr;
        else {
            x->prev->next=x->next;
            x->next->prev=x->prev;
            if (y->child==x) y->child=x->next;
        }
        y->degree--;
        x->parent=nullptr;
        x->marked=false;
        splice(x);
    };

    function<void(Node*)> cascading_node=[&](Node* y) {
        Node* z=y->parent;
        if (!z) return;
        if (!y->marked) y->marked=true;
        else { node(y, z); cascading_node(z); }
    };
    //Insert all nodes
    for (int i=0; i<n; i++) {
        node_ptr[i]=new Node(i, dist[i]);
        splice(node_ptr[i]);
        heap_size++;
    }

    while (min_node) {
        Node* z=min_node;
        //promote children to root list
        if (z->child) {
            vector<Node*> children;
            Node* c=z->child;
            do{ children.push_back(c); c=c->next; } while(c!=z->child);
            for (Node* ch: children) {ch->parent=nullptr; splice(ch);}
        }
        //remove z
        z->prev->next=z->next;
        z->next->prev=z->prev;
        min_node=(z==z->next)? nullptr: z->next;
        if (min_node) consolidate();
        heap_size--;

        int u=z->vertex;
        delete z;

        for (auto& [v,w]: neighbors[u]) {
            double nd=dist[u]+w;
            if (nd<dist[v]) {
                dist[v]=nd;
                prev[v]=u;
                //decrease key
                Node* x=node_ptr[v];
                x->key=nd;
                Node* p=x->parent;
                if (p && x->key<p->key) { node(x, p); cascading_node(p);}
                if (x->key<min_node->key) min_node=x;
            }
        }
    }

    // Reconstruct path by walking backwards from destination to source
    int destination = n - 1;

    if (dist[destination] == INF) {
        path.clear();
        return INF;
    }

    for (int node = destination; node != -1; node = prev[node]) {
        path.push_back(node);
    }
    reverse(path.begin(), path.end());

    return dist[destination];

}