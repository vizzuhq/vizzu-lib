#include <algorithm>
#include <cmath>
#include <limits>

#include "bubblechart_impl.h"

using namespace Vizzu;
using namespace Geom;
using namespace Vizzu::Charts;

BubbleChartImpl::BubbleChartImpl(const std::vector<double> &sizes,
    const Rect &rect)
{
	radiuses.reserve(sizes.size());

	for (auto j = 0u; j < sizes.size(); j++) {
		double radius = std::sqrt(sizes[j]);
		radiuses.push_back({j, radius});
	}

	std::sort(radiuses.begin(),
	    radiuses.end(),
	    [](const RadiusRecord &a, const RadiusRecord &b)
	    {
		    return b.value < a.value;
	    });

	data.reserve(radiuses.size());

	generate();
	normalize(rect);

	std::sort(data.begin(),
	    data.end(),
	    [](const DataRecord &a, const DataRecord &b)
	    {
		    return a.index < b.index;
	    });
}

void BubbleChartImpl::generate()
{
	auto baseIndex = 0u;

	for (auto i = 0u; i < radiuses.size(); i++) {
		auto &record = radiuses[i];

		switch (i)
		{
		case 0:
			data.emplace_back(record.index, Circle(Point(0, 0), record.value));
			break;

		case 1:
			data.emplace_back(record.index,
			    Circle(Point(radiuses[0].value + record.value, 0),
			        record.value));
			break;
	
		default:
			if (record.value == 0.0)
			{
				data.emplace_back(record.index, Circle(Point(0, 0), 0));
				continue;
			}

			auto candidate0 = getTouchingCircle(record, baseIndex, i - 1);
			auto candidate1 = getTouchingCircle(record, baseIndex+1, i - 1);

			if (candidate1
				&& !candidate1->overlaps(data[baseIndex].circle, 0.00001)) 
			{
				data.emplace_back(record.index, *candidate1);
				baseIndex++;
			}
			else if (candidate0
				&& !candidate0->overlaps(data[baseIndex+1].circle, 0.00001)) 
			{
				data.emplace_back(record.index, *candidate0);
			}
			else throw std::logic_error("Cannot generate bubble chart");

			break;
		}
	}
}

std::optional<Geom::Circle> BubbleChartImpl::getTouchingCircle(
	const RadiusRecord &act, 
	size_t firstIdx, 
	size_t lastIdx)
{
	if (firstIdx == lastIdx) return std::nullopt;

	auto first = data[firstIdx].circle;
	auto last = data[lastIdx].circle;

	first.radius += act.value;
	last.radius += act.value;

	auto newCenter = last.intersection(first);

	if (!newCenter) return std::nullopt;

	return Circle(*newCenter, act.value);
}
