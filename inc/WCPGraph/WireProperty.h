#ifndef WIRECELLGRAPH_WIREPROPERTY
#define WIRECELLGRAPH_WIREPROPERTY

#include "WCPData/GeomWire.h"
#include <boost/graph/adjacency_list.hpp>

namespace WCPGraph {

    struct WireProperty {
	const WCP::GeomWire* wire;
	WireProperty(const WCP::GeomWire* w=0) : wire(w) {};
    };
}

namespace boost { 
    namespace graph {

	template<>
	struct internal_vertex_name<WCPGraph::WireProperty>
	{
	    typedef multi_index::member<WCPGraph::WireProperty, 
					const WCP::GeomWire*, 
					&WCPGraph::WireProperty::wire> type;
	};

	template<>
	struct internal_vertex_constructor<WCPGraph::WireProperty>
	{
	    typedef vertex_from_name<WCPGraph::WireProperty> type;
	};
	
    }
} // end namespace boost::graph



#endif
