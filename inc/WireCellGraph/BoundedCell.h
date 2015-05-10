#ifndef WIRECELLDATA_BOUNDEDCELL
#define WIRECELLDATA_BOUNDEDCELL

#include "WireCellData/Point.h"
#include "WireCellData/GeomWire.h"

namespace WireCell {

    /// Simplest representation of a cell.
    struct BoundedCell {
	WireCell::PointVector bounds;
	WireCell::GeomWireSelection wires;
    };

    /// Collection of BoundedCells
    typedef std::vector<BoundedCell> BoundedCellSet;

}

#endif
