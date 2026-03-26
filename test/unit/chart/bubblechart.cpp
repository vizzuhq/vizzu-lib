#include "chart/speclayout/bubblechart.h"

#include <vector>

#include "../util/test.h"

using test::operator""_suite;
using test::check;
using test::operator""_is_true;

namespace
{
bool noOverlaps(const Vizzu::Charts::BubbleChart::Markers &markers)
{
	for (auto i = 0U; i < markers.size(); ++i) {
		if (markers[i].circle().radius == 0) continue;
		for (auto j = i + 1; j < markers.size(); ++j) {
			if (markers[j].circle().radius == 0) continue;
			if (markers[i].circle().overlaps(markers[j].circle()))
				return false;
		}
	}
	return true;
}
}

const static auto tests =
    "Chart::Bubblechart"_suite
    | "short fail" |
    []
    {
	    std::vector<double> areas;
	    areas.insert(areas.end(), 7, 1.0);
	    areas.insert(areas.end(), 34, 0.1);
	    auto &&bc{Vizzu::Charts::BubbleChart(areas)};
	    auto &&nextCirc = bc.markers.at(8).circle();
	    auto &&lastCirc = bc.markers.back().circle();

	    check->* (nextCirc.center - lastCirc.center).abs()
	        >= nextCirc.radius + lastCirc.radius;
	    check->* noOverlaps(bc.markers)
	        == "no circle overlaps"_is_true;
    }

    | "long fail" |
    []
    {
	    std::vector<double> areas;
	    areas.insert(areas.end(), 6, 1.0);
	    areas.insert(areas.end(), 1, 0.64);
	    areas.insert(areas.end(), 124, 0.008);
	    auto &&bc{Vizzu::Charts::BubbleChart(areas)};

	    auto &&nextCirc = bc.markers.at(12).circle();
	    auto &&lastCirc = bc.markers.back().circle();
	    check->* (nextCirc.center - lastCirc.center).abs()
	        >= nextCirc.radius + lastCirc.radius;
	    check->* noOverlaps(bc.markers)
	        == "no circle overlaps"_is_true;
    };
