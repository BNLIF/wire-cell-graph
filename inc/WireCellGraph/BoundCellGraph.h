#ifndef WIRECELLGRAPH_BOUNDCELLGRAPH
#define WIRECELLGRAPH_BOUNDCELLGRAPH

#include "WireCellGraph/PropertyGraph.h"
#include "WireCellGraph/WireCrossingGraph.h"
#include "WireCellGraph/PointProperty.h"

namespace WireCellGraph {

    class BoundCellGraph :  public PropertyGraph<WireCellGraph::PointProperty,
						 boost::no_property>
    {
    public:
	BoundCellGraph(WireCellGraph::WireCrossingGraph& wcg);
    private:
	WireCellGraph::WireCrossingGraph& wcg;
	void fill();
    };

}

#endif
