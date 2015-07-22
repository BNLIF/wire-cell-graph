#include "WireCellData/GeomWire.h"
#include "WireCellNav/ExampleGDS.h"
#include <boost/graph/adjacency_list.hpp>
#include <vector>

#include "boost/date_time/posix_time/posix_time_types.hpp"
using namespace boost::posix_time;

#include <iostream>
using namespace std;


enum vertex_properties_t { vertex_properties };
//    enum edge_properties_t { edge_properties };
namespace boost {
    BOOST_INSTALL_PROPERTY(vertex, properties);
//	BOOST_INSTALL_PROPERTY(edge, properties);
}


// needs to move into its own header
typedef std::pair<float,float> Position;
typedef std::vector<Position> PositionVector;
typedef boost::adjacency_list<boost::setS, boost::vecS, 
			      boost::undirectedS,
			      boost::property<vertex_properties_t, Position>,
			      boost::property<boost::edge_index_t, std::size_t> > PositionGraph;
typedef boost::graph_traits<PositionGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<PositionGraph>::edge_descriptor Edge;
typedef boost::property_map<PositionGraph, boost::edge_index_t>::type EdgeInd_Map;

enum EdgeType_t { unknown_edge=0, bound_edge, internal_edge, neighbor_edge };
enum VertexType_t { unknown_vertex=0, center_vertex, corner_vertex };


class CellGraph {
public:				       // public for development
    PositionGraph graph;	       // the actual graph data
    int nedges;			       // count number of edges
    EdgeInd_Map edge_ind;	       // edge->index lookup
    std::vector<EdgeType_t> edge_type; // hold type of edge 
    std::vector<VertexType_t> vertex_type; // what kind of vertex

public:
    CellGraph() 
	: graph()
	, nedges(0)
	, edge_ind(boost::get(boost::edge_index, graph))
	, edge_type()
	{};
    ~CellGraph() {/*pew pew*/};
    
    /// Convert from 3d point to 2d position in the transverse plane
    PositionVector dim3to2(WireCell::PointVector& points) {
	PositionVector ret;
	for (int ind=0; ind<points.size(); ++ind) {
	    ret.push_back(Position(points[ind].z, points[ind].y));
	}
	return ret;
    }

    /// Record one position
    void record(WireCell::PointVector& pcell) {

	PositionVector pos = dim3to2(pcell);

	size_t npos = pos.size(); // should be >0

	std::vector<Vertex> vertex;

	Position center(0,0);
	for (int ind=0; ind<npos; ++ind) {
	    center.first += pos[ind].first;
	    center.second += pos[ind].second;
	    Vertex v = boost::add_vertex(pos[ind], graph);
	    vertex.push_back(v); // find/make graph vertices
	    vertex_type.resize(v+1, unknown_vertex);
	    vertex_type[v] = corner_vertex;
	}
	center.first /= npos;
	center.second /= npos;
	Vertex cv = boost::add_vertex(center, graph);
	vertex_type.resize(cv+1, unknown_vertex);
	vertex_type[cv] = center_vertex;
	

	for (int ind=0; ind<npos; ++ind) {
	    int next_ind=(ind+1)%npos; // circle

	    // add sides of cell
	    std::pair<Edge,bool> side = boost::add_edge(vertex[ind], vertex[next_ind], nedges, graph);
	    if (side.second) {
		int ind = edge_ind[side.first];
		edge_type.resize(ind+1, unknown_edge);
		edge_type[ind] = bound_edge;
		++nedges;
	    }
	    // add center->corner of cell
	    std::pair<Edge,bool> intern = boost::add_edge(cv, vertex[ind], nedges, graph);
	    if (intern.second) {
		int ind = edge_ind[side.first];
		edge_type.resize(ind+1, unknown_edge);
		edge_type[ind] = internal_edge;
		++nedges;
	    }
	    else {
		cerr << "WARNING: encountered duplicate center-corner edge\n";
	    }
	}

    }

    void load(WireCell::GeomDataSource& gds) {
	using namespace WireCell;

	std::vector<float> area_property;
	std::vector<EdgeType_t> edge_types_property;

	WireCell::GeomWireSelection u_wires = gds.wires_in_plane(kUwire);
	WireCell::GeomWireSelection v_wires = gds.wires_in_plane(kVwire);
	WireCell::GeomWireSelection w_wires = gds.wires_in_plane(kYwire);

	float pitch_u = gds.pitch(kUwire);
	float pitch_v = gds.pitch(kVwire);
	float pitch_w = gds.pitch(kYwire);

	std::pair<int,int> box_ind[4] = { // allows loops over a box of indices
	    std::pair<int,int>(0,0),
	    std::pair<int,int>(0,1),
	    std::pair<int,int>(1,1),
	    std::pair<int,int>(1,0)
	};

	
	for (int u_ind=0; u_ind < u_wires.size(); ++u_ind) {
	    const WireCell::GeomWire& u_wire = *u_wires[u_ind];
	    float dis_u_wire = gds.wire_dist(u_wire);
	    float dis_u[2] = { dis_u_wire - pitch_u, dis_u_wire + pitch_u }; // half-line minmax

	    for (int v_ind=0; v_ind < v_wires.size(); ++v_ind) {
		const WireCell::GeomWire& v_wire = *v_wires[v_ind];
		float dis_v_wire = gds.wire_dist(v_wire);
		float dis_v[2] = { dis_v_wire - pitch_v, dis_v_wire + pitch_v };


		std::vector<Vector> puv(4);
		float dis_puv[4];
		for (int ind=0; ind<4; ++ind) {
		    // fixme: we are not handling the case where one
		    // of these crossing points are outside the wire
		    // plane boundary.
		    gds.crossing_point(dis_u[box_ind[ind].first], dis_v[box_ind[ind].second], kUwire, kVwire, puv[ind]);
		    dis_puv[ind] = gds.wire_dist(puv[ind], kYwire);
		}

		for (int w_ind=0; w_ind < w_wires.size(); ++w_ind) {
		    const WireCell::GeomWire& w_wire = *w_wires[w_ind];
		    float dis_w_wire = gds.wire_dist(w_wire);
		    float dis_w[2] = { dis_w_wire - pitch_w, dis_w_wire + pitch_w };

		    WireCell::PointVector pcell;

		    for (int ind=0; ind<4; ++ind) {
			if (dis_w[0] <= dis_puv[ind] && dis_puv[ind] < dis_w[1]) {
			    pcell.push_back(puv[ind]);
			}
		    }

		    if (!pcell.size()) { 
			continue;
		    }

		    for (int ind=0; ind<4; ++ind) {
			{		// fresh scope
			    WireCell::Vector pointvec;
			    if (gds.crossing_point(dis_u[box_ind[ind].first], dis_w[box_ind[ind].second], 
						   kUwire, kYwire, pointvec)) 
			    {
				float disv = gds.wire_dist(pointvec, kVwire);
				if (dis_v[0] <= disv && disv < dis_v[1]) {
				    pcell.push_back(pointvec);
				}
			    }
			}

			{		// fresh scope
			    WireCell::Vector pointvec;
			    if (gds.crossing_point(dis_v[box_ind[ind].first], dis_w[box_ind[ind].second], 
						   kVwire, kYwire, pointvec)) 
			    {
				float disu = gds.wire_dist(pointvec, kUwire);
				if (dis_u[0] <= disu && disu < dis_u[1]) {
				    pcell.push_back(pointvec);
				}
			    }
			}

		    }
		
		    this->record(pcell);


		} // W wires
	    } // v wires
	} // u wires

    }; // load
}; // class CellGraph

void dump_time(std::string msg, ptime t1, ptime t2)
{
    time_duration diff = t2-t1;
    cerr << msg << " " << 0.001*diff.total_milliseconds() << " seconds" << endl;
}

int main() {

    /* 
       Horrible scaling!

       Wire pitch: 10 mm
       Loaded: #vertices=269446 #edges=431120
       Made GDS in 0 seconds
       Made CG in 1.269 seconds

       Wire pitch: 3 mm
       Loaded: #vertices=2971026 #edges=4753648
       Made GDS in 0.03 seconds
       Made CG in 25.883 seconds


   After adding better caching in GDS::by_planeindex() and GDS::angle() and removing -ggdb3
       
       
Wire pitch: 10 mm
Loaded: #vertices=269446 #edges=431120
Made GDS in 0 seconds
Made CG in 0.135 seconds
Wire pitch: 5 mm
Loaded: #vertices=1070231 #edges=1712376
Made GDS in 0.028 seconds
Made CG in 0.594 seconds
Wire pitch: 3 mm
Loaded: #vertices=2971026 #edges=4753648
Made GDS in 0.113 seconds
Made CG in 1.923 seconds


    */

    double sizes[] = { 3*units::mm, 5*units::mm, 10*units::mm, -1 };
    for (int isize=2; sizes[isize]>0; ++isize) {
	ptime t1(microsec_clock::local_time());
	WireCell::GeomDataSource& gds = *WireCell::make_example_gds(sizes[isize]);
	ptime t2(microsec_clock::local_time());
	CellGraph cg;
	cg.load(gds);
	ptime t3(microsec_clock::local_time());
    
	cout << "Wire pitch: " << sizes[isize]/units::mm << " mm\n";
	cout << "Loaded: #vertices=" << boost::num_vertices(cg.graph) << " #edges=" << boost::num_edges(cg.graph) << endl;
	dump_time("Made GDS in",t1,t2);
	dump_time("Made CG in",t2,t3);	
    }

    return 0;

}
