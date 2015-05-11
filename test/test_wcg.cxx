#include "WireCellGraph/WireCrossingGraph.h"
#include "WireCellNav/ParamGDS.h"
#include "WireCellNav/ExampleGDS.h"

#include <iostream>
#include "boost/date_time/posix_time/posix_time_types.hpp"

using namespace boost::posix_time;

using namespace std;
using namespace WireCell;
using namespace WireCellGraph;

void dump_time(std::string msg, ptime t1, ptime t2)
{
    time_duration diff = t2-t1;
    cerr << msg << " " << 0.001*diff.total_milliseconds() << " seconds" << endl;
}

/** some benchmarking
   10mm before mm caching in GDS and with -ggdb3
   Made WCG in 168.492 seconds
   After mm caching fix in GDS
   Made WCG in 5.168 seconds
   After removing cout
   Made WCG in 4.925 seconds
   After removing -ggdb3
   Made WCG in 4.827 seconds

   #vertices: 826195
   #edges   : 780642

   Fix making of duplicate wire vertices:

   Made WCG in 0.594 seconds
   #wires: U: 116 V: 115 W: 100 
   #vertices: 331
   #edges   : 17844
   
   pow!

   Going from 10mm to 3mm pitch:
   Made WCG in 24.248 seconds
   #wires: U: 385 V: 381 W: 334 
   #vertices: 1100
   #edges   : 199081
   
   O(N^3)'ed....
 */

int main()
{
    ptime t1(microsec_clock::local_time());
    GeomDataSource& gds = *make_example_gds(10*units::mm);
    ptime t2(microsec_clock::local_time());
    WireCrossingGraph wcg(gds);
    ptime t3(microsec_clock::local_time());

    dump_time("Made GDS in",t1,t2);
    dump_time("Made WCG in",t2,t3);

    cerr << "#wires: " 
	 << "U: " << gds.wires_in_plane(kUwire).size() << " "
	 << "V: " << gds.wires_in_plane(kVwire).size() << " "
	 << "W: " << gds.wires_in_plane(kYwire).size() << " "
	 << endl;
    cerr << "#vertices: " << wcg.getVertexCount() << endl;
    cerr << "#edges   : " << wcg.getEdgeCount() << endl;

    WireCrossingGraph::vertex_iter vert_it, vert_start, vert_stop;
    boost::tie(vert_start, vert_stop) = wcg.getVertices();
    for (vert_it=vert_start; vert_it != vert_stop; ++vert_it) {
//	cerr << *vert_start << endl;
    }

    WireCrossingGraph::edge_iter edge_it, edge_start, edge_stop;
    boost::tie(edge_start,edge_stop) = wcg.getEdges();
    for (edge_it=edge_start; edge_it!=edge_stop; ++edge_it) {
//	cerr << *edge_it << endl;
    }
    
}
