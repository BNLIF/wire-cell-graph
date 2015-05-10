#ifndef WIRECELLGRAPH_PROPERTIES
#define WIRECELLGRAPH_PROPERTIES

#include <boost/graph/adjacency_list.hpp>

namespace WireCellGraph {

    struct WireProperty {
	const WireCell::GeomWire* wire;
	WireProperty(const WireCell::GeomWire* w) : wire(w) {};
    };

    struct PointProperty {
	WireCell::Point point;
	PointProperty(const WireCell::Point& p = WireCell::Point()) : point(p) {};
    };	

}
#endif

