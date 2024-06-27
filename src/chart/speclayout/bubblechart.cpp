#include "bubblechart.h"

#include <algorithm>
#include <cmath>

namespace Vizzu::Charts
{

BubbleChart::BubbleChart(const std::vector<double> &circleAreas,
    const SpecMarker *parent)
{
	markers.reserve(circleAreas.size());

	for (auto j = 0U; const auto &circleArea : circleAreas)
		markers.emplace_back(j++,
		    std::sqrt(std::abs(circleArea)),
		    std::signbit(circleArea));

	std::sort(markers.begin(), markers.end(), SpecMarker::sizeOrder);

	generate();
	normalize(parent ? parent->circle().boundary()
	                 : Geom::Rect{{}, Geom::Size{1, 1}});

	std::sort(markers.begin(), markers.end(), SpecMarker::indexOrder);
}

void BubbleChart::generate()
{
	auto baseMarker = markers.begin();
	while (baseMarker != markers.end()
	       && !std::isfinite(baseMarker->size()))
		baseMarker++->emplaceCircle(Geom::Point{0, 0}, 0);
	while (baseMarker != markers.end() && baseMarker->negative)
		baseMarker++->emplaceCircle(Geom::Point{0, 0}, 0);
	if (baseMarker == markers.end()) return;

	auto firstMarkerSize = baseMarker->size();
	baseMarker->emplaceCircle(Geom::Point{0, 0}, firstMarkerSize);

	auto nextBaseMarker = baseMarker + 1;
	while (
	    nextBaseMarker != markers.end() && nextBaseMarker->negative)
		nextBaseMarker++->emplaceCircle(Geom::Point{0, 0}, 0);
	if (nextBaseMarker == markers.end()) return;

	auto currMarker = nextBaseMarker;
	auto markerSize = currMarker->size();
	currMarker->emplaceCircle(
	    Geom::Point{firstMarkerSize + markerSize, 0},
	    markerSize);

	for (auto preMarker = currMarker++; currMarker != markers.end();
	     ++currMarker) {
		if (currMarker->negative) {
			currMarker->emplaceCircle(Geom::Point{0, 0}, 0);
			continue;
		}

		markerSize = currMarker->size();
		if (markerSize == 0.0) break;

		if (auto &&candidate1 = getTouchingCircle(markerSize,
		        *nextBaseMarker,
		        *preMarker);
		    candidate1
		    && !candidate1->overlaps(baseMarker->circle(), 0.00001)) {
			currMarker->emplaceCircle(*candidate1);
			baseMarker = nextBaseMarker++;
			while (nextBaseMarker->negative) ++nextBaseMarker;
		}
		else if (auto &&candidate0 = getTouchingCircle(markerSize,
		             *baseMarker,
		             *preMarker);
		         candidate0
		         && !candidate0->overlaps(nextBaseMarker->circle(),
		             0.00001))
			currMarker->emplaceCircle(*candidate0);
		else {
			// TODO bubblechart generation failed. It need a fix.
			break;
		}
		preMarker = currMarker;
	}

	while (currMarker != markers.end())
		currMarker++->emplaceCircle(Geom::Point{0, 0}, 0);
}

void BubbleChart::normalize(const Geom::Rect &rect)
{
	if (markers.empty()) return;

	auto bound = markers[0].circle().boundary();
	for (const auto &marker : markers)
		bound = bound.boundary(marker.circle().boundary());

	auto maxSize = std::max(bound.width(), bound.height());
	auto &&cMul = rect.size / maxSize;
	auto &&radMul = rect.size.minSize() / maxSize;
	for (auto &&center = rect.center(); auto &marker : markers)
		marker.emplaceCircle(
		    center + cMul * (marker.circle().center - bound.center()),
		    marker.circle().radius * radMul);
}

std::optional<Geom::Circle> BubbleChart::getTouchingCircle(
    double newMarkerSize,
    const SpecMarker &firstMarker,
    const SpecMarker &lastMarker)
{
	if (&firstMarker == &lastMarker) return {};
	auto first = firstMarker.circle();
	auto last = lastMarker.circle();

	first.radius += newMarkerSize;
	last.radius += newMarkerSize;

	if (const auto newCenter = last.intersection(first)[0])
		return Geom::Circle(*newCenter, newMarkerSize);

	return {};
}

}