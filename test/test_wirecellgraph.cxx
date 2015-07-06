#include "WireCellData/GeomWire.h"
#include "WireCellNav/ExampleWires.h"
#include "WireCellNav/GenerativeFDS.h"
#include "WireCellNav/DillDepositor.h"
#include "WireCellGraph/WireCellGraph.h"
#include "WireCellNav/SliceDataSource.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"

#include <iostream>

using namespace WireCell;
using namespace WireCellGraph;
using namespace std;
using namespace boost::posix_time;

void dump_time(std::string msg, ptime t1, ptime t2)
{
    time_duration diff = t2-t1;
    cerr << msg << " " << 0.001*diff.total_milliseconds() << " seconds" << endl;
}

int main()
{
    const double wire_pitch = 5*units::mm;
    ptime t1(microsec_clock::local_time());
    WireCell::IWireGeometry* wires = WireCell::make_example_wires(wire_pitch);
    WireCell::GeomDataSource gds;
    gds.use_wires(*wires);
    ptime t2(microsec_clock::local_time());
    WCGraphs cg;
    cg.load(gds);
    ptime t3(microsec_clock::local_time());
    
    const float width = 1.0*units::m;
    const DillDepositor::MinMax drift_dim(0, width), trans_dim(-0.5*width,0.5*width);
    const DillDepositor::MinMax length(1.0*units::mm, 1.0*units::cm);

    DillDepositor dill(drift_dim, trans_dim, trans_dim, length);
    GenerativeFDS fds(dill, gds);
    

    cout << "Wire pitch: " << wire_pitch/units::mm << " mm\n";
    cout << "Loaded: #vertices=" << boost::num_vertices(cg.graph) << " #edges=" << boost::num_edges(cg.graph) << endl;
    dump_time("Made GDS in",t1,t2);
    dump_time("Made CG in",t2,t3);	


    for (int ind=0; ind<10; ++ind) {
	ptime t_loop1(microsec_clock::local_time());

	fds.jump(ind);
	Frame frame = fds.get();
	cerr << "FRAME["<<frame.index<<"] with " << frame.traces.size() << " traces" << endl;

	const SimTruthSelection truth = fds.truth();
	cerr << "TRUTH["<<ind<<"] has " << truth.size() << " hits" << endl;

	for (int tind=0; tind<truth.size(); ++tind) {
	    //cerr << "\t# " << tind 
	    //<< ": q=" << truth[tind]->charge() << " @ " << truth[tind]->pos() << endl;
	}

	SliceDataSource sds(fds);

	while (sds.next() >= 0) {
	    const Slice& slice = sds.get();
	    const Channel::Group& group = slice.group();
	}
	ptime t_loop2(microsec_clock::local_time());
	dump_time("Loop",t_loop1, t_loop2);	
    }

    return 0;
}
