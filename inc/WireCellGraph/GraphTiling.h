#if 0

#ifndef WIRECELLGRAPH_GRAPHTILING
#define WIRECELLGRAPH_GRAPHTILING

#include "WireCellNav/GeomDataSource.h"
#include "WireCellGraph/WireCrossingGraph.h"
#include "WireCellGraph/CellBoundsGraph.h"

namespace WireCellGraph {

    void fill_graphs(WireCell::GeomDataSource& gds, 
		     WireCrossingGraph& wcg,
		     CellBoundsGraph& cbg);


    /** GraphTiling - produce a tiling represented as a few interrelated graphs.
     */
    class GraphTiling {
    public: 
	GraphTiling(WireCell::GeomDataSource& gds);
	virtual ~GraphTiling();
	
    private:

    };
}

#endif
#endif
