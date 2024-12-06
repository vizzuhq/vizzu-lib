#include "plot.h"

#include <compare>
#include <cstddef>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "base/anim/interpolated.h"
#include "chart/main/style.h"
#include "chart/options/options.h"

#include "marker.h"

namespace Vizzu::Gen
{

Plot::MarkersInfo interpolate(const Plot::MarkersInfo &op1,
    const Plot::MarkersInfo &op2,
    double factor)
{
	Plot::MarkersInfo result;
	auto iter1 = op1.begin();
	auto iter2 = op2.begin();
	for (; iter1 != op1.end() && iter2 != op2.end();
	     ++iter1, ++iter2) {
		if (iter1->first != iter2->first)
			throw std::logic_error("invalid map operation");
		if (iter1->second.get() || iter2->second.get()) {
			result.insert(std::make_pair(iter1->first,
			    interpolate(iter1->second, iter2->second, factor)));
		}
	}
	return result;
}

Plot::MarkerInfoContent::MarkerInfoContent(const Marker &marker) :
    markerId{marker.idx},
    info{marker.cellInfo, &marker.cellInfo->markerInfo}
{}

Plot::MarkerInfoContent::operator bool() const
{
	return markerId.has_value();
}

bool Plot::MarkerInfoContent::operator==(
    const MarkerInfoContent &op) const
{
	return markerId == op.markerId;
}

Plot::Plot(PlotOptionsPtr opts, Styles::Chart style) :
    guides(*opts),
    options(std::move(opts)),
    style(std::move(style))
{}

void Plot::detachOptions()
{
	options = std::make_shared<Gen::Options>(*options);
}

bool Plot::isEmpty() const
{
	return options->getChannels().isEmpty();
}

Geom::Rect Plot::getMarkersBounds() const
{
	auto markerIt = markers.begin();
	while (markerIt != markers.end()
	       && !static_cast<bool>(markerIt->enabled))
		++markerIt;

	if (markerIt == markers.end()) return {};

	auto boundRect = markerIt->toRectangle().positive();
	while (++markerIt != markers.end())
		if (markerIt->enabled)
			boundRect = boundRect.boundary(markerIt->toRectangle());
	return boundRect;
}

void Plot::prependMarkers(const Plot &plot)
{
	auto it = markers.insert(markers.begin(),
	              plot.getMarkers().begin(),
	              plot.getMarkers().end())
	        + static_cast<std::ptrdiff_t>(plot.markers.size());

	for (auto i = markers.begin(); i < it; ++i) i->enabled = false;
}

void Plot::appendMarkers(const Plot &plot)
{
	for (auto it = markers.insert(markers.end(),
	         plot.getMarkers().begin(),
	         plot.getMarkers().end());
	     it != markers.end();
	     ++it)
		it->enabled = false;
}

bool Plot::dimensionMatch(const Plot &a, const Plot &b)
{
	return a.getOptions()->getChannels().getDimensions()
	    == b.getOptions()->getChannels().getDimensions();
}
bool Plot::hasMarkerChange(const Plot &source, const Plot &target)
{
	auto msize = source.markers.size();
	if (msize != target.markers.size()
	    || source.markersInfo != target.markersInfo)
		return true;

	for (std::size_t ix = 0; ix < msize; ++ix)
		if (source.markers[ix].idx != target.markers[ix].idx)
			return true;
	return false;
}
void Plot::mergeMarkersAndCellInfo(Plot &source, Plot &target)
{
	auto &&smarkers = source.markers;
	auto &&tmarkers = target.markers;

	std::map<Marker::MarkerIndex, std::ptrdiff_t> indices;

	auto first1 = smarkers.begin();
	auto first2 = tmarkers.begin();
	std::ptrdiff_t newIx{};
	while (first1 != smarkers.end() && first2 != tmarkers.end()) {
		if (auto cmp = first1->idx <=> first2->idx; std::is_lt(cmp)) {
			indices.try_emplace(first1->idx, newIx);
			first2 = tmarkers.emplace(first2, *first1);
			first2->enabled = false;
		}
		else if (std::is_gt(cmp)) {
			indices.try_emplace(first2->idx, newIx);
			first1 = smarkers.emplace(first1, *first2);
			first1->enabled = false;
		}
		else
			indices.try_emplace(first1->idx, newIx);
		++first1;
		++first2;
		++newIx;
	}

	if (first2 != tmarkers.end())
		while (first2 != tmarkers.end()) {
			indices.try_emplace(first2->idx, newIx++);
			smarkers.emplace_back(*first2++).enabled = false;
		}
	else
		while (first1 != smarkers.end()) {
			indices.try_emplace(first1->idx, newIx++);
			tmarkers.emplace_back(*first1++).enabled = false;
		}

	const auto markers_size =
	    static_cast<std::ptrdiff_t>(indices.size());
	for (std::ptrdiff_t ix{}; ix < markers_size; ++ix) {
		auto &smarker = smarkers[ix];
		auto &tmarker = tmarkers[ix];
		if (auto &[idx, prePos] = smarker.prevMainMarker->value;
		    !idx.empty())
			prePos = indices.at(idx) - ix;

		if (auto &[idx, prePos] = tmarker.prevMainMarker->value;
		    !idx.empty())
			prePos = indices.at(idx) - ix;

		if (auto &scell = smarker.cellInfo, &tcell = tmarker.cellInfo;
		    scell && !tcell)
			tcell = scell;
		else if (!scell && tcell)
			scell = tcell;
	}
}

}
