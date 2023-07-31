#ifndef BUBBLECHART_IMPL_H
#define BUBBLECHART_IMPL_H

#include <cstddef>
#include <map>
#include <unordered_map>

#include "bubblechart.h"

namespace Vizzu::Charts
{

class BubbleChartImpl : public BubbleChart
{
public:
	explicit BubbleChartImpl(const std::vector<double> &sizes,
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
	
	std::optional<Geom::Circle> getTouchingCircle(
		const RadiusRecord &act, 
		size_t firstIdx, 
		size_t lastIdx);
};

}

#endif
