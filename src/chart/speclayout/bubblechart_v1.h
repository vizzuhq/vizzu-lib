#ifndef BUBBLECHART_V1_H
#define BUBBLECHART_V1_H

#include <cstddef>
#include <map>
#include <unordered_map>

#include "bubblechart.h"

namespace Vizzu
{
namespace Charts
{

class BubbleChartV1 : public BubbleChart
{
public:
	BubbleChartV1(const std::vector<double> &sizes,
	    Boundary boundary,
	    const Geom::Rect &rect = Geom::Rect(Geom::Point(0, 0),
	        Geom::Size(1, 1)));

private:
	struct RadiusRecord
	{
		size_t index;
		double value;
	};

	std::vector<RadiusRecord> radiuses;

	Boundary boundary;
	Geom::Point center;
	double allSize;

	void generate();
	void addCircle(size_t index, const Geom::Circle &circle);
	void updateCenter(const Geom::Circle &circle);
	double sqrCenterDistance(const Geom::Point &p) const;
	bool overlapsAny(const Geom::Circle &circle) const;
};

}
}

#endif
