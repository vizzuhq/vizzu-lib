#ifndef BUBBLECHART_H
#define BUBBLECHART_H

#include <vector>

#include "base/geom/circle.h"
#include "base/geom/point.h"

#include "specmarker.h"

namespace Vizzu
{
namespace Charts
{

class BubbleChart
{
public:

	typedef std::vector<SpecMarker> Data;

	const Data &getData() const { return data; }

protected:
	Data data;

	void normalize(const Geom::Rect &rect);
};

}
}

#endif
