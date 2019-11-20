#ifndef WIRECELLGRAPH_WIRECROSSINGGRAPH
#define WIRECELLGRAPH_WIRECROSSINGGRAPH

#include "WCPGraph/PropertyGraph.h"
#include "WCPGraph/WireProperty.h"
#include "WCPGraph/PointProperty.h"

#include "WCPNav/GeomDataSource.h"

namespace WCPGraph {

    /** WireCrossingGraph - a graph with vertices representing wires
     * and edges representing their crossing points.
     */
    typedef PropertyGraph<WCPGraph::WireProperty, 
			  WCPGraph::PointProperty> WireCrossingGraph;

    void fill_wirecrossinggraph(WCP::GeomDataSource& gds, 
				WireCrossingGraph& wcg);
}
#endif



