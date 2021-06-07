#include "bubblechart_v1.h"

#include <algorithm>
#include <cmath>
#include <limits>

using namespace Vizzu;
using namespace Geom;
using namespace Vizzu::Charts;

BubbleChartV1::BubbleChartV1(const std::vector<double> &sizes, Boundary boundary, const Rect &rect)
	: boundary(boundary)
{
	center = Point(0, 0);
	allSize = 0;

	for (auto j = 0u; j < sizes.size(); j++) {
		double radius = std::sqrt(sizes[j]);
		radiuses.push_back({ j, radius });
	}

	std::sort(radiuses.begin(), radiuses.end(),
	[](const RadiusRecord &a, const RadiusRecord &b) {
		return b.value < a.value;
	});

	generate();
	normalize(rect);

	std::sort(data.begin(), data.end(),
	[](const DataRecord &a, const DataRecord &b) {
		return a.index < b.index;
	});
}

void BubbleChartV1::generate()
{
	for (auto i = 0u; i < radiuses.size(); i++)
	{
		auto &record = radiuses[i];

		if (i == 0) addCircle(record.index, Circle(Point(0,0), record.value));

		if (i == 1)
			addCircle(record.index, Circle(
				Point(0, radiuses[0].value + record.value), record.value));

		if (i >= 2)
		{
			bool found = false;
			Circle foundCircle;
			auto minDistance = std::numeric_limits<double>::max();

			if (record.value > 0.0)
				for(auto j = 0u; j < i-1; j++)
					for(auto k = j+1u; k < i; k++)
						for(auto s = -1; s <= 1; s+=2)
			{
				if (data[j].circle.radius == 0 || data[k].circle.radius == 0)
					continue;

				if (data[j].circle.distance(data[k].circle) < 1.9 * record.value)
				{
					auto circle = Circle(data[j].circle, data[k].circle, record.value, s);
					auto distance = sqrCenterDistance(circle.center);

					if(!overlapsAny(circle)
					   && (!found || distance < minDistance))
					{
						foundCircle = circle;
						minDistance = distance;
						found = true;
					}
				}
			}
			if (!found && record.value > 0.0)
				throw std::logic_error("internal error: cannot generate bubble chart");
			addCircle(record.index, foundCircle);
		}
	}
}

void BubbleChartV1::addCircle(size_t index, const Circle &circle)
{
	updateCenter(circle);
	data.push_back({ index, circle });
}

void BubbleChartV1::updateCenter(const Circle &circle)
{
	auto area = circle.area();

	if (area >= 0.0)
		center = (center * allSize + circle.center * area)
				/ (allSize + area);

	allSize += area;
}

double BubbleChartV1::sqrCenterDistance(const Point &p) const
{
	auto d = p - center;

	if (boundary == Boundary::Diamond)
		return d.manhattan();
	if (boundary == Boundary::Box)
		return d.chebyshev();
	else
		return d.sqrAbs();
}

bool BubbleChartV1::overlapsAny(const Circle &circle) const
{
	for(const auto &record : data)
		if (record.circle.radius > 0
			&& record.circle.overlaps(circle, .001))
			return true;
	return false;
}
