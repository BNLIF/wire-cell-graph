#include "WireCellGraph/WireCellGraph.h"
#include <iostream>

using namespace std;
using namespace WireCell;
using namespace WireCellGraph;

WCGraphs::WCGraphs() 
	: graph()
	, nedges(0)
	, edge_ind(boost::get(boost::edge_index, graph))
	, edge_type()
	{};

WCGraphs::~WCGraphs() {/*pew pew*/};
    


/// Convert from 3d point to 2d position in the transverse plane
WCGraphs::PositionVector WCGraphs::dim3to2(WireCell::PointVector& points)
{
    PositionVector ret;
    for (int ind=0; ind<points.size(); ++ind) {
	ret.push_back(Position(points[ind].z, points[ind].y));
    }
    return ret;
}

/// Record one position
void WCGraphs::record(WireCell::PointVector& pcell)
{
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
} // record()

void WCGraphs::load(WireCell::GeomDataSource& gds)
{
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
} // load()
