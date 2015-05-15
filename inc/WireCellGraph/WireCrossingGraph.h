#ifndef WIRECELLGRAPH_WIRECROSSINGGRAPH
#define WIRECELLGRAPH_WIRECROSSINGGRAPH

#include "WireCellGraph/PropertyGraph.h"
#include "WireCellGraph/WireProperty.h"
#include "WireCellGraph/PointProperty.h"

#include "WireCellNav/GeomDataSource.h"

namespace WireCellGraph {

    /** WireCrossingGraph - a graph with vertices representing wires
     * and edges representing their crossing points.
     */
    typedef PropertyGraph<WireCellGraph::WireProperty, 
			  WireCellGraph::PointProperty> WireCrossingGraph;

    void fill_wirecrossinggraph(WireCell::GeomDataSource& gds, 
				WireCrossingGraph& wcg);
}
#endif



