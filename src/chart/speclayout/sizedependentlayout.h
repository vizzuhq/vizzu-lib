//
// Created by bela on 2024.05.17..
//

#ifndef SIZEDEPENDENTLAYOUT_H
#define SIZEDEPENDENTLAYOUT_H

namespace Vizzu::Charts
{
template <class ChartType> struct SizeDependentLayout
{
	template <class Hierarchy, class AfterMarkerSetter>
	static void setupVector(const Hierarchy &hierarchy,
	    AfterMarkerSetter &&afterMarkerSetter)
	{
		if (hierarchy.empty()) return;

		std::vector<double> sizes(hierarchy.size());
		for (std::size_t ix{}; const auto &level : hierarchy)
			for (auto &sum = sizes[ix++]; const auto &item : level) {
				if (!item) continue;
				if (auto &&size = item->sizeFactor;
				    std::isfinite(size) && size > 0)
					sum += size;
			}

		const ChartType chart(sizes);

		for (auto it = chart.markers.data();
		     const auto &level : hierarchy) {
			std::vector<double> ssizes(level.size());
			for (std::size_t ix{}; const auto &item : level) {
				if (item) ssizes[ix] = item->sizeFactor;
				++ix;
			}

			const ChartType subChart(ssizes, it++);

			for (std::size_t subCnt{}; const auto &item : level) {
				if (item)
					afterMarkerSetter(*item,
					    subChart.markers[subCnt]);
				++subCnt;
			}
		}
	}
};
}

#endif // SIZEDEPENDENTLAYOUT_H
