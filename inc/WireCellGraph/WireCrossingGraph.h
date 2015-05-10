#ifndef WIRECELLGRAPH_WIRECROSSINGGRAPH
#define WIRECELLGRAPH_WIRECROSSINGGRAPH

#include "WireCellGraph/PropertyGraph.h"
#include "WireCellGraph/WireProperty.h"
#include "WireCellGraph/PointProperty.h"

#include "WireCellNav/GeomDataSource.h"

namespace WireCellGraph {

    class WireCrossingGraph : public PropertyGraph<WireCellGraph::WireProperty, 
						   WireCellGraph::PointProperty>
    {
    public:
	WireCrossingGraph(WireCell::GeomDataSource& gds);

    private:
	WireCell::GeomDataSource& gds;
	void fill();

    };

};

#endif
