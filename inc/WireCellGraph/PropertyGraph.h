#ifndef WIRECELLGRAPH_PROPERTYGRAPH
#define WIRECELLGRAPH_PROPERTYGRAPH

#include <boost/graph/adjacency_list.hpp>

/* definition of basic boost::graph properties */
enum vertex_properties_t { vertex_properties };
enum edge_properties_t { edge_properties };
namespace boost {
	BOOST_INSTALL_PROPERTY(vertex, properties);
	BOOST_INSTALL_PROPERTY(edge, properties);
}

namespace WireCellGraph {

    // taken from http://stackoverflow.com/questions/671714/modifying-vertex-properties-in-a-boostgraph
    /// A class that manages a graph with properties associated with vertices and edges.
    template < typename VERTEXPROPERTIES, typename EDGEPROPERTIES >
    class PropertyGraph
    {
    public:
	typedef boost::adjacency_list< boost::setS, boost::vecS, boost::undirectedS, 
				       boost::property<vertex_properties_t, VERTEXPROPERTIES>,
				       boost::property<edge_properties_t, EDGEPROPERTIES>
				       > GraphContainer;

	typedef typename boost::graph_traits<GraphContainer>::vertex_descriptor Vertex;
	typedef typename boost::graph_traits<GraphContainer>::edge_descriptor Edge;
	typedef std::pair<Edge, Edge> EdgePair;

	typedef typename boost::graph_traits<GraphContainer>::vertex_iterator vertex_iter;
	typedef typename boost::graph_traits<GraphContainer>::edge_iterator edge_iter;
	typedef typename boost::graph_traits<GraphContainer>::adjacency_iterator adjacency_iter;
	typedef typename boost::graph_traits<GraphContainer>::out_edge_iterator out_edge_iter;

	typedef typename boost::graph_traits<GraphContainer>::degree_size_type degree_t;

	typedef std::pair<adjacency_iter, adjacency_iter> adjacency_vertex_range_t;
	typedef std::pair<out_edge_iter, out_edge_iter> out_edge_range_t;
	typedef std::pair<vertex_iter, vertex_iter> vertex_range_t;
	typedef std::pair<edge_iter, edge_iter> edge_range_t;


	PropertyGraph() {}

	PropertyGraph(const PropertyGraph& g) : graph(g.graph) {}

	virtual ~PropertyGraph() {}

	void Clear() {
	    graph.clear();
	}

	Vertex AddVertex(const VERTEXPROPERTIES& prop) {
	    return add_vertex(prop, graph);
	    //Vertex v = add_vertex(graph);
	    //properties(v) = prop;
	    //return v;
	}

	void RemoveVertex(const Vertex& v) {
	    clear_vertex(v, graph);
	    remove_vertex(v, graph);
	}

	Edge AddEdge(const Vertex& v1, const Vertex& v2, const EDGEPROPERTIES& prop)
	{
	    /* TODO: maybe one wants to check if this edge could be inserted */
	    Edge edge = add_edge(v1, v2, prop, graph).first;
	    //properties(edge) = prop;
	    return edge;
	}

	/* property access */
	VERTEXPROPERTIES& properties(const Vertex& v) 
	{
	    typename boost::property_map<GraphContainer, vertex_properties_t>::type param = get(vertex_properties, graph);
	    return param[v];
	}

	const VERTEXPROPERTIES& properties(const Vertex& v) const 
	{
	    typename boost::property_map<GraphContainer, vertex_properties_t>::const_type param = get(vertex_properties, graph);
	    return param[v];
	}

	EDGEPROPERTIES& properties(const Edge& v)
	{
	    typename boost::property_map<GraphContainer, edge_properties_t>::type param = get(edge_properties, graph);
	    return param[v];
	}

	const EDGEPROPERTIES& properties(const Edge& v) const
	{
	    typename boost::property_map<GraphContainer, edge_properties_t>::const_type param = get(edge_properties, graph);
	    return param[v];
	}


	/* selectors and properties */
	const GraphContainer& getGraph() const
	{
	    return graph;
	}

	vertex_range_t getVertices() const
	{
	    return vertices(graph);
	}

	adjacency_vertex_range_t getAdjacentVertices(const Vertex& v) const
	{
	    return adjacent_vertices(v, graph);
	}

	int getVertexCount() const
	{
	    return num_vertices(graph);
	}

	int getVertexDegree(const Vertex& v) const
	{
	    return out_degree(v, graph);
	}


	int getEdgeCount() const
	{
	    return num_edges(graph);
	}

	edge_range_t getEdges() const
	{
	    return edges(graph);
	}

	/* operators */
	PropertyGraph& operator=(const PropertyGraph &rhs)
	{
	    graph = rhs.graph;
	    return *this;
	}

    protected:
	GraphContainer graph;
    };
}


#endif 
