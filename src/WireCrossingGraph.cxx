#include "WCPGraph/WireCrossingGraph.h"

#include "WCPData/GeomWire.h"

#include <iostream> // debug

using namespace WCPGraph;

void WCPGraph::fill_wirecrossinggraph(WCP::GeomDataSource& gds, 
					   WireCrossingGraph& wcg)
{
    WCP::GeomWireSelection u_wires = gds.wires_in_plane(WCP::kUwire);
    WCP::GeomWireSelection v_wires = gds.wires_in_plane(WCP::kVwire);
    WCP::GeomWireSelection w_wires = gds.wires_in_plane(WCP::kYwire);

    // std::cerr << "U:" << u_wires.size() << " "
    // 	      << "V:" << v_wires.size() << " "
    // 	      << "W:" << w_wires.size() << " "
    // 	      << std::endl;


    std::vector<WireCrossingGraph::Vertex> u_vertices, v_vertices, w_vertices;
    for (int u_ind=0; u_ind < u_wires.size(); ++u_ind) {
	u_vertices.push_back(wcg.AddVertex(WireProperty(u_wires[u_ind])));
    }
    for (int v_ind=0; v_ind < v_wires.size(); ++v_ind) {
	v_vertices.push_back(wcg.AddVertex(WireProperty(v_wires[v_ind])));
    }
    for (int w_ind=0; w_ind < w_wires.size(); ++w_ind) {
	w_vertices.push_back(wcg.AddVertex(WireProperty(w_wires[w_ind])));
    }

    for (int u_ind=0; u_ind < u_wires.size(); ++u_ind) {
	WireCrossingGraph::Vertex& u_vertex = u_vertices[u_ind];
	const WCP::GeomWire& u_wire = *u_wires[u_ind];

	for (int v_ind=0; v_ind < v_wires.size(); ++v_ind) {
	    WireCrossingGraph::Vertex& v_vertex = v_vertices[v_ind];
	    const WCP::GeomWire& v_wire = *v_wires[v_ind];

	    WCP::Vector pt_uv;
	    if (! gds.crossing_point(u_wire, v_wire, pt_uv)) {
		continue;
	    }

	    wcg.AddEdge(u_vertex, v_vertex, PointProperty(pt_uv));

	    for (int w_ind=0; w_ind < w_wires.size(); ++w_ind) {
		WireCrossingGraph::Vertex& w_vertex = w_vertices[w_ind];
		const WCP::GeomWire& w_wire = *w_wires[w_ind];
    
		WCP::Vector pt_uw;
		if (gds.crossing_point(u_wire, w_wire, pt_uw)) {
		    wcg.AddEdge(u_vertex, w_vertex, PointProperty(pt_uw));
		}
		
		WCP::Vector pt_vw;
		if (gds.crossing_point(v_wire, w_wire, pt_vw)) {
		    wcg.AddEdge(v_vertex, w_vertex, PointProperty(pt_vw));
		}
	    }
	}
    }
}

