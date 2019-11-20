#ifndef WIRECELLDATA_BOUNDEDCELL
#define WIRECELLDATA_BOUNDEDCELL

#include "WCPData/Point.h"
#include "WCPData/GeomWire.h"

namespace WCP {

    /// Simplest representation of a cell.
    struct BoundedCell {
	WCP::PointVector bounds;
	WCP::GeomWireSelection wires;
    };

    /// Collection of BoundedCells
    typedef std::vector<BoundedCell> BoundedCellSet;

}

#endif
