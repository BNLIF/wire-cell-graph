#include "WCPGraph/BoundCellGraph.h"

using namespace WCPGraph;

BoundCellGraph::BoundCellGraph(WireCrossingGraph& wcg)
    : PropertyGraph()
    , wcg(wcg)
{
    this->fill();
}

void BoundCellGraph::fill()
{
    // WireCrossingGraph::edge_range_t xpt_iter, xpt_start, xpt_stop;
    // boost::tie(xpt_start, xpt_stop) = wcg.getEdges();
    // for (xpt_iter=xptr_start; xpt_iter!=xpt_stop; ++xpt_iter) {
    // 	const PointProperty& pp = wcg.properties(*xptr_ietr);
    // 	Point pp.point
	
    // }
    

    
}
