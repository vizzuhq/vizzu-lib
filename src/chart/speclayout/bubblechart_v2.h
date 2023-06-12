#ifndef BUBBLECHART_V2_H
#define BUBBLECHART_V2_H

#include "bubblechart.h"

namespace Vizzu
{
namespace Charts
{

template <class ChartType> class BubbleChartBuilder;

class BubbleChartV2 : public BubbleChart
{
public:
	BubbleChartV2(const std::vector<double> &sizes,
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

	void generate();

	static Geom::Point touchingBubbleCenter(Geom::Circle circle1,
	    Geom::Circle circle2,
	    double radius3);

	static double distanceBetweenCircles(const Geom::Circle &circle1,
	    const Geom::Circle &circle2);

	static double calculateSide(const Geom::Point &point1,
	    const Geom::Point &point2,
	    const Geom::Point &point3);
};

}
}

#endif
