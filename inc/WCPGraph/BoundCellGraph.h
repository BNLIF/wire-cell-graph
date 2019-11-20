#ifndef WIRECELLGRAPH_BOUNDCELLGRAPH
#define WIRECELLGRAPH_BOUNDCELLGRAPH

#include "WCPGraph/PropertyGraph.h"
#include "WCPGraph/WireCrossingGraph.h"
#include "WCPGraph/PointProperty.h"

namespace WCPGraph {

    class BoundCellGraph :  public PropertyGraph<WCPGraph::PointProperty,
						 boost::no_property>
    {
    public:
	BoundCellGraph(WCPGraph::WireCrossingGraph& wcg);
    private:
	WCPGraph::WireCrossingGraph& wcg;
	void fill();
    };

}

#endif
