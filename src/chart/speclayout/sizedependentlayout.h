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
			for (auto &sum = sizes[ix++]; const auto &item : level)
				if (item->sizeFactor > 0) sum += item->sizeFactor;

		const ChartType chart(sizes);

		std::vector<double> ssizes(hierarchy.inner_size());

		for (auto it = chart.markers.data();
		     const auto &level : hierarchy) {
			for (std::size_t ix{}; const auto &item : level)
				ssizes[ix++] = item->sizeFactor;

			const ChartType subChart(ssizes, it++);

			for (std::size_t subCnt{}; const auto &item : level)
				afterMarkerSetter(item, subChart.markers[subCnt++]);
		}
	}
};
}

#endif // SIZEDEPENDENTLAYOUT_H
