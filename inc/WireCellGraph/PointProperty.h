#ifndef WIRECELLGRAPH_POINTPROPERTY
#define WIRECELLGRAPH_POINTPROPERTY

#include "WireCellData/Point.h"

namespace WireCellGraph {

    struct PointProperty {
	const WireCell::Point point;
	PointProperty() : point() {};
	PointProperty(const WireCell::Point& p) : point(p) {};
    };
}

namespace boost { 
    namespace graph {

	template<>
	struct internal_vertex_name<WireCellGraph::PointProperty>
	{
	    typedef multi_index::member<WireCellGraph::PointProperty, 
					const WireCell::Point, 
					&WireCellGraph::PointProperty::point> type;
	};

	template<>
	struct internal_vertex_constructor<WireCellGraph::PointProperty>
	{
	    typedef vertex_from_name<WireCellGraph::PointProperty> type;
	};
	
    }
} // end namespace boost::graph

#endif
