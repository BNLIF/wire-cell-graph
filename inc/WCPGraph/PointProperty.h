#ifndef WIRECELLGRAPH_POINTPROPERTY
#define WIRECELLGRAPH_POINTPROPERTY

#include "WCPData/Point.h"

namespace WCPGraph {

    struct PointProperty {
	const WCP::Point point;
	PointProperty() : point() {};
	PointProperty(const WCP::Point& p) : point(p) {};
    };
}

namespace boost { 
    namespace graph {

	template<>
	struct internal_vertex_name<WCPGraph::PointProperty>
	{
	    typedef multi_index::member<WCPGraph::PointProperty, 
					const WCP::Point, 
					&WCPGraph::PointProperty::point> type;
	};

	template<>
	struct internal_vertex_constructor<WCPGraph::PointProperty>
	{
	    typedef vertex_from_name<WCPGraph::PointProperty> type;
	};
	
    }
} // end namespace boost::graph

#endif
