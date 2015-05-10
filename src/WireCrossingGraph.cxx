#include "WireCellGraph/WireCrossingGraph.h"

#include "WireCellData/GeomWire.h"

#include <iostream> // debug

using namespace WireCellGraph;

WireCrossingGraph::WireCrossingGraph(WireCell::GeomDataSource& gds)
    : PropertyGraph()
    , gds(gds)
{
    this->fill();
}

void WireCrossingGraph::fill()
{
    WireCell::GeomWireSelection u_wires = gds.wires_in_plane(WireCell::kUwire);
    WireCell::GeomWireSelection v_wires = gds.wires_in_plane(WireCell::kVwire);
    WireCell::GeomWireSelection w_wires = gds.wires_in_plane(WireCell::kYwire);

    // std::cerr << "U:" << u_wires.size() << " "
    // 	      << "V:" << v_wires.size() << " "
    // 	      << "W:" << w_wires.size() << " "
    // 	      << std::endl;


    std::vector<Vertex> u_vertices, v_vertices, w_vertices;
    for (int u_ind=0; u_ind < u_wires.size(); ++u_ind) {
	u_vertices.push_back(AddVertex(WireProperty(u_wires[u_ind])));
    }
    for (int v_ind=0; v_ind < v_wires.size(); ++v_ind) {
	v_vertices.push_back(AddVertex(WireProperty(v_wires[v_ind])));
    }
    for (int w_ind=0; w_ind < w_wires.size(); ++w_ind) {
	w_vertices.push_back(AddVertex(WireProperty(w_wires[w_ind])));
    }

    for (int u_ind=0; u_ind < u_wires.size(); ++u_ind) {
	Vertex& u_vertex = u_vertices[u_ind];
	const WireCell::GeomWire& u_wire = *u_wires[u_ind];

	for (int v_ind=0; v_ind < v_wires.size(); ++v_ind) {
	    Vertex& v_vertex = v_vertices[v_ind];
	    const WireCell::GeomWire& v_wire = *v_wires[v_ind];

	    WireCell::Point pt_uv;
	    if (! gds.crossing_point(u_wire, v_wire, pt_uv)) {
		continue;
	    }

	    AddEdge(u_vertex, v_vertex, PointProperty(pt_uv));

	    for (int w_ind=0; w_ind < w_wires.size(); ++w_ind) {
		Vertex& w_vertex = w_vertices[w_ind];
		const WireCell::GeomWire& w_wire = *w_wires[w_ind];
    
		WireCell::Point pt_uw;
		if (gds.crossing_point(u_wire, w_wire, pt_uw)) {
		    AddEdge(u_vertex, w_vertex, PointProperty(pt_uw));
		}
		
		WireCell::Point pt_vw;
		if (gds.crossing_point(v_wire, w_wire, pt_vw)) {
		    AddEdge(v_vertex, w_vertex, PointProperty(pt_vw));
		}
	    }
	}
    }
}

