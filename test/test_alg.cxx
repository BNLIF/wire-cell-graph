// Chao Zhang, chao@bnl.gov
// test BGL algorithms
// many examples can be found at http://programmingexamples.net/wiki/Boost/BGL

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// #include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

struct VertexProp {
    int id;
    // add pointer to merged cell
};

struct EdgeProp {
    float dist; // edge distance
};

// define a merged cell undirected graph with properties
typedef adjacency_list<vecS, vecS, undirectedS, VertexProp, EdgeProp> MCUGraph;
typedef graph_traits<MCUGraph>::vertex_descriptor vertex_descriptor;
typedef graph_traits<MCUGraph>::edge_descriptor edge_descriptor;

const int N = 10;

void construct_graph(MCUGraph &g);
void test_dijkstra_shortest_paths(MCUGraph &g);

int main()
{
    MCUGraph g(N); // this created a graph with N vertex (or call add_vertex N times)

    cout << "constructing graph ..." << endl;
    construct_graph(g);

    cout << "\ntesting dijkstra_shortest_paths ..." << endl;
    test_dijkstra_shortest_paths(g);

}

void construct_graph(MCUGraph &g)
{
    // accessing vetex
    for (int i=0; i<N; i++) {
        auto v = vertex(i, g); // retrieve vertex descriptor
        g[v].id = i*i; // dummy assign vertex property
        if (i==N-1) {
            cout << "vertex " << v << ": id " << g[v].id << endl;
        }
    }

    // add edges
    for (int i=0; i<N-2; i++) {
        auto e = add_edge(i, i+1, g); // add edge for one to the next
        if (e.second) { // first is the descriptor, 2nd is bool indicating if add_edge succeeded
            g[e.first].dist = 2; // dummy assign the edge property.
        }

        e = add_edge(i, i+2, g); // add another edge
        if (e.second) { // first is the descriptor, 2nd is bool indicating if add_edge succeeded
            g[e.first].dist = 1; // dummy assign the edge property.
        }
    }

    // accessing edge 2
    auto e = edge(2, 3, g).first;
    cout << "edge from " << source(e, g) << " to " << target(e, g) << " has dist: " << g[e].dist << endl;
    e = edge(2, 4, g).first;
    cout << "edge from " << source(e, g) << " to " << target(e, g) << " has dist: " << g[e].dist << endl;

}

void test_dijkstra_shortest_paths(MCUGraph &g)
{
    // Create things for Dijkstra
    vector<vertex_descriptor> parents(num_vertices(g)); // To store parents
    vector<int> distances(num_vertices(g)); // To store distances

    // Compute shortest paths from v0 to all vertices, and store the output in parents and distances
    auto v0 = vertex(0, g);
    boost::dijkstra_shortest_paths(g, v0,
        weight_map(get(&EdgeProp::dist, g))
        .predecessor_map(&parents[0])
        .distance_map(&distances[0])
    );

    // Output all vertice results
    for (int i=0; i<N; i++) {
        cout << "distance(0, " << i << ") = " << distances[i] << ", ";
        cout << "parent(" << i << ") = " << parents[i] << endl;
    }
    // another way to iterate vertices
    // graph_traits<MCUGraph>::vertex_iterator it, vend;
    // for (tie(it, vend) = vertices(g); it != vend; ++it) {
    //     cout << "distance(" << *it << ") = " << distances[*it] << ", ";
    //     cout << "parent(" << *it << ") = " << parents[*it] << endl;
    // }

    // output shortest path from vertex 0 to 9:
    int dest = N-1; // start at the destination and walk back to the source
    vector<int> path;
    path.push_back(dest);
    for(int i = dest; i!=0; i = parents[i]) {
        path.push_back(i);
    }
    path.push_back(0);
    cout << "shortest path from 0 to " << N-1 << ":";
    for (int i=path.size()-1; i!=0; i--) {
        cout << " " << path[i];
    }
    cout << endl;
}
