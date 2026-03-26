#include "bubblechart.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>
#include <vector>

#include "base/geom/point.h"
#include "base/geom/rect.h"

#include "specmarker.h"

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

	std::ranges::stable_sort(markers, SpecMarker::sizeOrder);

	generate();
	normalize(parent ? parent->circle().boundary()
	                 : Geom::Rect{{}, Geom::Size{1, 1}});

	std::ranges::stable_sort(markers, SpecMarker::indexOrder);
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

	// Per-marker state for O(n log n) overlap detection.
	// Circles are sorted descending by radius, so children are always
	// placed after (higher index than) their parent.
	const auto n = static_cast<int>(markers.size());
	std::vector<int> parentOf(n, -1);
	std::vector<int> nextOfPar(n, -1); // N at the time this marker became B
	std::vector<int> nextSib(n, -1);
	std::vector<int> lastChild(n, -1);
	// children[i]: {angle-from-i-center, childIdx}, in placement
	// (descending-angle) order
	std::vector<std::vector<std::pair<double, int>>> children(n);

	auto mkIdx = [&](Markers::iterator it) -> int
	{
		return static_cast<int>(it - markers.begin());
	};

	auto addChild = [&](int pIdx, int cIdx)
	{
		parentOf[cIdx] = pIdx;
		if (lastChild[pIdx] != -1) nextSib[lastChild[pIdx]] = cIdx;
		lastChild[pIdx] = cIdx;
		auto dc = markers[cIdx].circle().center
		        - markers[pIdx].circle().center;
		children[pIdx].emplace_back(std::atan2(dc.y, dc.x), cIdx);
	};

	// Binary search in children[pnIdx] (stored in descending-angle
	// order) for a child whose circle overlaps cand.  Checks the
	// hit position and its two immediate neighbours.
	auto findChildOv =
	    [&](int pnIdx, const Geom::Circle &cand) -> int
	{
		if (pnIdx < 0 || pnIdx >= n) return -1;
		auto &ch = children[pnIdx];
		if (ch.empty()) return -1;
		auto dc =
		    cand.center - markers[pnIdx].circle().center;
		double a = std::atan2(dc.y, dc.x);
		// Descending order: lower_bound with (ang < c.first)
		// finds the first element where c.first <= a.
		auto it = std::lower_bound(ch.begin(),
		    ch.end(),
		    a,
		    [](const auto &c, double ang)
		    {
			    return ang < c.first;
		    });
		for (int d : {0, -1, 1}) {
			auto jt = it + d;
			if (jt >= ch.begin() && jt < ch.end())
				if (cand.overlaps(markers[jt->second].circle()))
					return jt->second;
		}
		return -1;
	};

	// Initialise state for the first two placed markers.
	{
		int b0 = mkIdx(baseMarker);
		int n0 = mkIdx(nextBaseMarker);
		parentOf[n0] = b0;
		lastChild[b0] = n0;
		nextOfPar[b0] = n0;
		auto dc = nextBaseMarker->circle().center
		        - baseMarker->circle().center;
		children[b0].emplace_back(std::atan2(dc.y, dc.x), n0);
	}

	for (auto preMarker = currMarker++; currMarker != markers.end();
	     ++currMarker) {
		if (currMarker->negative) {
			currMarker->emplaceCircle(Geom::Point{0, 0}, 0);
			continue;
		}

		markerSize = currMarker->size();
		if (markerSize == 0.0) break;

		bool placed = false;
		while (!placed) {
			int bIdx = mkIdx(baseMarker);
			int nIdx = mkIdx(nextBaseMarker);

			auto candidate1 = getTouchingCircle(markerSize,
			    *nextBaseMarker,
			    *preMarker);

			// cand1: touches nextBaseMarker and preMarker
			// Basic check: must not overlap baseMarker or sorted[0]
			if (candidate1
			    && !candidate1->overlaps(baseMarker->circle())
			    && !candidate1->overlaps(markers[0].circle())) {

				// nextSib[N] check
				{
					int ns = nextSib[nIdx];
					if (ns != -1
					    && candidate1->overlaps(
					        markers[ns].circle())) {
						baseMarker = markers.begin() + ns;
						continue;
					}
				}
				// children[parent(B).nextOf] check
				{
					int pnB = (parentOf[bIdx] >= 0)
					            ? nextOfPar[parentOf[bIdx]]
					            : -1;
					int ov = findChildOv(pnB, *candidate1);
					if (ov >= 0) {
						baseMarker = markers.begin() + ov;
						continue;
					}
				}
				// children[B.nextOf.parent.nextOf] check
				{
					int nB = nextOfPar[bIdx];
					if (nB >= 0 && nB < n) {
						int pnB2 =
						    (parentOf[nB] >= 0)
						        ? nextOfPar[parentOf[nB]]
						        : -1;
						int ov = findChildOv(pnB2, *candidate1);
						if (ov >= 0) {
							baseMarker = markers.begin() + ov;
							continue;
						}
					}
				}

				currMarker->emplaceCircle(*candidate1);
				addChild(nIdx, mkIdx(currMarker));
				auto oldN = nextBaseMarker;
				baseMarker = nextBaseMarker++;
				while (nextBaseMarker->negative) ++nextBaseMarker;
				nextOfPar[mkIdx(oldN)] = mkIdx(nextBaseMarker);
				placed = true;
			}
			else {
				auto candidate0 = getTouchingCircle(markerSize,
				    *baseMarker,
				    *preMarker);

				// cand0: touches baseMarker and preMarker
				// Basic check: must not overlap nextBaseMarker or
				// sorted[0]
				if (candidate0
				    && !candidate0->overlaps(
				        nextBaseMarker->circle())
				    && !candidate0->overlaps(markers[0].circle())) {

					// nextSib[B] check
					{
						int bNS = nextSib[bIdx];
						if (bNS != -1
						    && candidate0->overlaps(
						        markers[bNS].circle())) {
							baseMarker = markers.begin() + bNS;
							continue;
						}
					}
					// children[parent(B).nextOf] check
					{
						int pnB = (parentOf[bIdx] >= 0)
						            ? nextOfPar[parentOf[bIdx]]
						            : -1;
						int ov = findChildOv(pnB, *candidate0);
						if (ov >= 0) {
							baseMarker = markers.begin() + ov;
							continue;
						}
					}
					// children[B.nextOf.parent.nextOf] check
					{
						int nB = nextOfPar[bIdx];
						if (nB >= 0 && nB < n) {
							int pnB2 =
							    (parentOf[nB] >= 0)
							        ? nextOfPar[parentOf[nB]]
							        : -1;
							int ov = findChildOv(pnB2, *candidate0);
							if (ov >= 0) {
								baseMarker = markers.begin() + ov;
								continue;
							}
						}
					}

					currMarker->emplaceCircle(*candidate0);
					addChild(bIdx, mkIdx(currMarker));
					placed = true;
				}
				else {
					auto newNextBase =
					    std::next(nextBaseMarker);
					while (newNextBase != currMarker
					       && (newNextBase->negative
					           || newNextBase->circle().radius
					                  == 0))
						++newNextBase;
					if (newNextBase == currMarker) break;
					baseMarker = nextBaseMarker;
					nextBaseMarker = newNextBase;
				}
			}
		}

		if (!placed) break;
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