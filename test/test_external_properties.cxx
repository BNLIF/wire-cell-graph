/** 
 * test using external properties.
 */ 


#include <boost/graph/adjacency_list.hpp>
#include <vector>

#include <iostream>
using namespace std;


int main(){

    typedef boost::adjacency_list<boost::setS, boost::vecS, 
				  boost::undirectedS,
				  boost::no_property,
				  boost::property<boost::edge_index_t, std::size_t> > Graph;

    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;
    typedef boost::property_map<Graph, boost::edge_index_t>::type EdgeID_Map;

    typedef std::vector<double> ValueMap;

    Graph graph;
    ValueMap vert_map;
    
    EdgeID_Map edge_id = get(boost::edge_index, graph);

    for (int ind=0; ind < 10; ++ind) {
	Edge e = boost::add_edge(ind,ind+1, ind, graph).first;
	std::size_t si = boost::source(e, graph);
	std::size_t ti = boost::target(e, graph);
	vert_map.resize(si+1, 0.0);
	vert_map[si] = -1.0*ind;
	cout << ind << ": " << edge_id[e] 
	     << " " << si << " --> " << ti << " = " << vert_map[si]
	     << endl;
    }
    
    for (int ind=0; ind < vert_map.size(); ++ind) {
	cout << ind << " vert: " << vert_map[ind] << endl;
    }


    cout << "Add existing edge" << endl;

    Edge e;
    bool ok=false;

    boost::tie(e,ok) = boost::add_edge(2,3, 100, graph);
    cout << ok << " " << edge_id[e] << endl;
    boost::tie(e,ok) = boost::add_edge(3,2, 101, graph);
    cout << ok << " " << edge_id[e] << endl;
    boost::tie(e,ok) = boost::add_edge(2,4, 102, graph);
    cout << ok << " " << edge_id[e] << endl;

    return 0;
}
