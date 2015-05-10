// test boost graph library

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

// http://stackoverflow.com/questions/671714/modifying-vertex-properties-in-a-boostgraph
// http://blog.hostilefork.com/boost-graph-templates-not-crazy/

struct City {
    std::string name;
    unsigned population;
    City(std::string n="", unsigned p=0) : name(n), population(p) {};
    
//	template< class Archive >
//	void serialize(Archive & ar, const unsigned int version)
//	    { ar & name; };
};
    
struct Road {
    std::string name;
    float miles;
    Road(std::string n="", float m=0.0) : name(n), miles(m) {};
//	template< class Archive >
//	void serialize(Archive & ar, const unsigned int version)
//	    { ar & name; };
};

// http://svn.boost.org/svn/boost/trunk/libs/graph/test/named_vertices_test.cpp
namespace boost {
    namespace graph {
	
	// Use the City name as a key for indexing cities in a graph
	template<>
	struct internal_vertex_name<City>
	{
	    typedef multi_index::member<City, std::string, &City::name> type;
	};
	
	// Allow the graph to build cities given only their names
	// (filling in the defaults for fields).
	template<>
	struct internal_vertex_constructor<City>
	{
	    typedef vertex_from_name<City> type;
	};
	
    } 
} // end namespace boost::graph

int main()
{
    using namespace std;
    using namespace boost;


    typedef adjacency_list<setS, vecS, undirectedS > UndirectedIntGraph;
    typedef adjacency_list<vecS, vecS, undirectedS, City, Road > CitiesAndRoadsGraph;
    typedef graph_traits<CitiesAndRoadsGraph>::vertex_descriptor CityVertex;
    typedef graph_traits<CitiesAndRoadsGraph>::edge_descriptor RoadEdge; 



    CitiesAndRoadsGraph car;

    CityVertex one = add_vertex(car);
    car[one].name="City One";
    car[one].population=100;

    std::cout << "#1: " << one << std::endl;

    CityVertex two = add_vertex(car);
    car[two].name="City Two";
    car[two].population=200;

    std::cout << "#2: " << two << std::endl;

    CityVertex three = add_vertex(City("City 3", 300), car);

    std::cout << "#3: " << car[three].name << std::endl;

    CityVertex threeprime = add_vertex(City("City 3", 300), car);

    std::cout << "#3': " << car[threeprime].name << std::endl;

    std::pair<RoadEdge, bool> road12 = add_edge(one, two, car);
    if (road12.second) {
	car[road12.first].name="Road from One to Two";
	car[road12.first].miles = 6.9;
	std::cout << "okay to make edge: " << road12.first
		  << ": " << road12.second
		  << std::endl;
    }
    else {
	std::cout << "failed to make edge" << std::endl;
    }
	
    add_edge(one, three, car);
    std::cout
	<< "#1: " << car[one].name << " "
	<< "#2: " << car[two].name << " "
	<< "12: " << car[road12.first].name << " "
	<< std::endl;

    std::cout 
	<< "#vertices:" << num_vertices(car) << endl
	<< "#edges   :" << num_edges(car) << endl;


    UndirectedIntGraph g;

    add_edge(5,1, g);
    add_edge(1,5, g); 
    add_edge(5,1, g);
    add_edge(5,2, g);
    add_edge(5,3, g);

    std::cout << num_edges(g) << "\n";

    std::cout << "got 0,3: " << edge(0,3, g).first << endl;
    std::cout << "got 0,3: " << edge(0,3, g).second << endl;
    std::cout << "got 3,0: " << edge(3,0, g).second << endl;
    std::cout << "got 0,4: " << edge(0,4, g).second << endl;
    std::cout << "got 5,1: " << edge(5,1, g).second << endl;
    std::cout << "got 1,5: " << edge(1,5, g).second << endl;
    std::cout << "got 2,5: " << edge(2,5, g).second << endl;
    std::cout << "got 5,3: " << edge(5,3, g).second << endl;

    typedef graph_traits<UndirectedIntGraph>::edge_iterator edge_iterator;

    std::pair<edge_iterator, edge_iterator> ei = edges(g);
    for(edge_iterator edge_iter = ei.first; edge_iter != ei.second; ++edge_iter) {
        std::cout << "(" << source(*edge_iter, g) << ", " << target(*edge_iter, g) << ")\n";
    }

    std::cout << "\n";

    return 0;
}
