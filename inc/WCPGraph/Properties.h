#ifndef WIRECELLGRAPH_PROPERTIES
#define WIRECELLGRAPH_PROPERTIES

#include <boost/graph/adjacency_list.hpp>

namespace WCPGraph {

    struct WireProperty {
	const WCP::GeomWire* wire;
	WireProperty(const WCP::GeomWire* w) : wire(w) {};
    };

    struct PointProperty {
	WCP::Point point;
	PointProperty(const WCP::Point& p = WCP::Point()) : point(p) {};
    };	

}
#endif

