#ifndef WIRECELLGRAPH_WIREPROPERTY
#define WIRECELLGRAPH_WIREPROPERTY

#include "WireCellData/GeomWire.h"
#include <boost/graph/adjacency_list.hpp>

namespace WireCellGraph {

    struct WireProperty {
	const WireCell::GeomWire* wire;
	WireProperty(const WireCell::GeomWire* w=0) : wire(w) {};
    };
}

namespace boost { 
    namespace graph {

	template<>
	struct internal_vertex_name<WireCellGraph::WireProperty>
	{
	    typedef multi_index::member<WireCellGraph::WireProperty, 
					const WireCell::GeomWire*, 
					&WireCellGraph::WireProperty::wire> type;
	};

	template<>
	struct internal_vertex_constructor<WireCellGraph::WireProperty>
	{
	    typedef vertex_from_name<WireCellGraph::WireProperty> type;
	};
	
    }
} // end namespace boost::graph



#endif
