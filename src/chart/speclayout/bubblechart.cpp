#include "bubblechart.h"

#include <algorithm>
#include <cmath>

namespace Vizzu::Charts
{

BubbleChart::BubbleChart(const std::vector<double> &circleAreas,
    const Geom::Rect &rect)
{
	markers.reserve(circleAreas.size());

	for (auto j = 0U; j < circleAreas.size(); ++j)
		markers.emplace_back(j,
		    std::sqrt(std::max(0.0, circleAreas[j])));

	std::sort(markers.begin(), markers.end(), SpecMarker::sizeOrder);

	generate();
	normalize(rect);

	std::sort(markers.begin(), markers.end(), SpecMarker::indexOrder);
}

void BubbleChart::generate()
{
	auto baseIndex = 0U;

	auto firstMarkerSize = markers.empty() ? 0.0 : markers[0].size();
	for (auto i = 0U; i < markers.size(); ++i) {
		auto &marker = markers[i];
		auto markerSize = marker.size();

		switch (i) {
		case 0:
			marker.emplaceCircle(Geom::Point{0, 0}, markerSize);
			break;

		case 1:
			marker.emplaceCircle(
			    Geom::Point{firstMarkerSize + markerSize, 0},
			    markerSize);
			break;

		default:
			if (markerSize == 0.0) {
				marker.emplaceCircle(Geom::Point{0, 0}, 0);
				continue;
			}

			auto candidate0 =
			    getTouchingCircle(marker, baseIndex, i - 1);
			auto candidate1 =
			    getTouchingCircle(marker, baseIndex + 1, i - 1);

			if (candidate1
			    && !candidate1->overlaps(markers[baseIndex].circle(),
			        0.00001)) {
				marker.emplaceCircle(*candidate1);
				++baseIndex;
			}
			else if (candidate0
			         && !candidate0->overlaps(
			             markers[baseIndex + 1].circle(),
			             0.00001)) {
				marker.emplaceCircle(*candidate0);
			}
			else
				throw std::logic_error(
				    "Cannot generate bubble chart");

			break;
		}
	}
}

void BubbleChart::normalize(const Geom::Rect &rect)
{
	if (markers.empty()) return;

	Geom::Rect bound = markers[0].circle().boundary();

	for (auto &marker : markers)
		bound = bound.boundary(marker.circle().boundary());

	auto maxSize = std::max(bound.width(), bound.height());

	auto center = rect.center();

	for (auto &marker : markers) {
		auto normCenter =
		    center
		    + rect.size * (marker.circle().center - bound.center())
		          / maxSize;

		auto normRadius =
		    marker.circle().radius * rect.size.minSize() / maxSize;

		marker.emplaceCircle(Geom::Circle(normCenter, normRadius));
	}
}

std::optional<Geom::Circle> BubbleChart::getTouchingCircle(
    const SpecMarker &act,
    size_t firstIdx,
    size_t lastIdx) const
{
	if (firstIdx == lastIdx) return std::nullopt;

	auto first = markers[firstIdx].circle();
	auto last = markers[lastIdx].circle();

	auto &&size = act.size();
	first.radius += size;
	last.radius += size;

	auto newCenter = last.intersection(first)[0];

	if (!newCenter) return std::nullopt;

	return Geom::Circle(*newCenter, size);
}

}