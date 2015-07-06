#include "WireCellGraph/WireCrossingGraph.h"
#include "WireCellNav/ExampleWires.h"

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

int main()
{
    const double wire_pitch = 5*units::mm;
    IWireGeometry* wires = make_example_wires(wire_pitch);
    GeomDataSource gds;
    gds.use_wires(*wires);
    WireCrossingGraph wcg;
    WireCellGraph::fill_wirecrossinggraph(gds, wcg);
}
