#ifndef WIRECELLGRAPH_WIRECROSSING
#define WIRECELLGRAPH_WIRECROSSING

#include <boost/graph/adjacency_list.hpp>

namespace WireCellGraph {

struct WireRef {
    const WireCell::GeomWire* wire;
    WireRef(const WireCell::GeomWire* w) : wire(w) {};
}

#endif

