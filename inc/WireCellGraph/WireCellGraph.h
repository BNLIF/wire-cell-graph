#ifndef WIRECELLGRAPH_WIRECELLGRAPH
#define WIRECELLGRAPH_WIRECELLGRAPH

#include "WireCellData/Point.h"
#include "WireCellNav/GeomDataSource.h"

#include <boost/graph/adjacency_list.hpp>
#include <vector>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <iostream>

namespace WireCellGraph {


    enum vertex_properties_t { vertex_properties };
    enum edge_properties_t { edge_properties };
//namespace boost {
//    BOOST_INSTALL_PROPERTY(vertex, properties);
////	BOOST_INSTALL_PROPERTY(edge, properties);
//}


    class WCGraphs {
    public:

	typedef std::pair<float,float> Position;
	typedef std::vector<Position> PositionVector;
	typedef boost::adjacency_list<boost::setS, boost::vecS, 
				      boost::undirectedS,
				      Position,
//				      boost::property<vertex_properties_t, Position>,
				      boost::property<boost::edge_index_t, std::size_t> > PositionGraph;
	typedef boost::graph_traits<PositionGraph>::vertex_descriptor Vertex;
	typedef boost::graph_traits<PositionGraph>::edge_descriptor Edge;
	typedef boost::property_map<PositionGraph, boost::edge_index_t>::type EdgeInd_Map;

	enum EdgeType_t { unknown_edge=0, bound_edge, internal_edge, neighbor_edge };
	enum VertexType_t { unknown_vertex=0, center_vertex, corner_vertex };



	WCGraphs();
	virtual ~WCGraphs();
    
	/// Convert from 3d point to 2d position in the transverse plane
	PositionVector dim3to2(WireCell::PointVector& points);
	void record(WireCell::PointVector& pcell);
	void load(WireCell::GeomDataSource& gds);

    public:					   // fixme, make this private
	PositionGraph graph;	       // the actual graph data
	int nedges;			       // count number of edges
	EdgeInd_Map edge_ind;	       // edge->index lookup
	std::vector<EdgeType_t> edge_type; // hold type of edge 
	std::vector<VertexType_t> vertex_type; // what kind of vertex

    }; // class WireCellGraph

} // namespace WireCellGraph
#endif
