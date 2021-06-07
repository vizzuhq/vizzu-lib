#include "bubblechart.h"

using namespace Vizzu;
using namespace Vizzu::Charts;

void BubbleChart::normalize(const Geom::Rect &rect)
{
	if (data.empty()) return;

	Geom::Rect bound = data[0].circle.boundary();

	for (auto &record : data)
		bound = bound.boundary(record.circle.boundary());

	auto maxSize = std::max(bound.width(), bound.height());

	auto center = rect.center();

	for (auto &record : data)
	{
		record.circle.center =
			center + rect.size * (record.circle.center - bound.center()) / maxSize;
		record.circle.radius =
			record.circle.radius * rect.size.minSize() / maxSize;
	}
}
