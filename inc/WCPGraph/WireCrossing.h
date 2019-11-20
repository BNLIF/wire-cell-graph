#ifndef WIRECELLGRAPH_WIRECROSSING
#define WIRECELLGRAPH_WIRECROSSING

#include <boost/graph/adjacency_list.hpp>

namespace WCPGraph {

struct WireRef {
    const WCP::GeomWire* wire;
    WireRef(const WCP::GeomWire* w) : wire(w) {};
}

#endif

