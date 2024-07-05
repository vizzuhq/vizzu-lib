#include "plot.h"

#include <limits>
#include <numeric>

#include "base/anim/interpolated.h"
#include "base/conv/numtostr.h"
#include "base/math/range.h"
#include "chart/speclayout/bubblechartbuilder.h"
#include "chart/speclayout/tablechart.h"
#include "chart/speclayout/treemap.h"

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
    markerId{std::in_place, marker.idx, marker.pos},
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
    anyAxisSet(opts->getChannels().anyAxisSet()),
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

void Plot::prependMarkers(const Plot &plot)
{
	auto size = plot.markers.size();

	auto it = markers.insert(markers.begin(),
	              plot.getMarkers().begin(),
	              plot.getMarkers().end())
	        + static_cast<std::ptrdiff_t>(size);

	for (auto i = markers.begin(); i < it; ++i) i->enabled = false;

	while (it != markers.end()) it++->setIdOffset(size);

	for (MarkerInfo &markerInfo :
	    std::ranges::views::values(markersInfo))
		if (auto &id = markerInfo.values[0].value.markerId)
			id->pos += size;
}

void Plot::appendMarkers(const Plot &plot)
{
	auto size = markers.size();

	for (auto it = markers.insert(markers.end(),
	         plot.getMarkers().begin(),
	         plot.getMarkers().end());
	     it != markers.end();
	     ++it) {
		auto &marker = *it;

		marker.setIdOffset(size);
		marker.enabled = false;
	}
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
	auto first1 = smarkers.begin();
	auto first2 = tmarkers.begin();
	std::vector<Marker::MarkerIndexPosition> source_reindex(
	    smarkers.size());
	std::vector<Marker::MarkerIndexPosition> target_reindex(
	    tmarkers.size());
	auto reindex1 = source_reindex.begin();
	auto reindex2 = target_reindex.begin();
	std::size_t pos{};
	while (first1 != smarkers.end() && first2 != tmarkers.end()) {
		if (auto cmp = first1->idx <=> first2->idx; std::is_lt(cmp)) {
			*reindex1++ = {first1->idx, pos};
			first2 = tmarkers.emplace(first2, *first1);
			first2->enabled = false;
		}
		else if (std::is_gt(cmp)) {
			*reindex2++ = {first2->idx, pos};
			first1 = smarkers.emplace(first1, *first2);
			first1->enabled = false;
		}
		else {
			*reindex1++ = *reindex2++ = {first1->idx, pos};
		}
		++first1;
		++first2;
		++pos;
	}

	if (first2 != tmarkers.end())
		while (first2 != tmarkers.end()) {
			*reindex2++ = {first2->idx, pos++};
			smarkers.emplace_back(*first2++).enabled = false;
		}
	else
		while (first1 != smarkers.end()) {
			*reindex1++ = {first1->idx, pos++};
			tmarkers.emplace_back(*first1++).enabled = false;
		}

	const auto markers_size =
	    std::max(smarkers.size(), tmarkers.size());
	for (std::size_t ix{}; ix < markers_size; ++ix) {
		auto &smarker = smarkers[ix];
		if (auto &[idx, prePos] =
		        smarker.prevMainMarker.values[0].value;
		    idx != ~Marker::MarkerIndex{}) {
			if (prePos < source_reindex.size()
			    && source_reindex[prePos].idx == idx)
				prePos = source_reindex[prePos].pos;
			else if (prePos < target_reindex.size()
			         && target_reindex[prePos].idx == idx)
				prePos = target_reindex[prePos].pos;
			else
				throw std::runtime_error(
				    "Logical error in marker reindex");
		}

		auto &tmarker = tmarkers[ix];
		if (auto &[idx, prePos] =
		        tmarker.prevMainMarker.values[0].value;
		    idx != ~Marker::MarkerIndex{}) {
			if (prePos < target_reindex.size()
			    && target_reindex[prePos].idx == idx)
				prePos = target_reindex[prePos].pos;
			else if (prePos < source_reindex.size()
			         && source_reindex[prePos].idx == idx)
				prePos = source_reindex[prePos].pos;
			else
				throw std::runtime_error(
				    "Logical error in marker reindex");
		}

		if (auto &scell = source.markers[ix].cellInfo,
		    &tcell = target.markers[ix].cellInfo;
		    scell && !tcell)
			tcell = scell;
		else if (!scell && tcell)
			scell = tcell;
	}

	for (MarkerInfo &markerInfo :
	    std::ranges::views::values(source.markersInfo))
		if (auto &id = markerInfo.values[0].value.markerId)
			id->pos = source_reindex[id->pos].pos;

	for (MarkerInfo &markerInfo :
	    std::ranges::views::values(target.markersInfo))
		if (auto &id = markerInfo.values[0].value.markerId)
			id->pos = target_reindex[id->pos].pos;
}

}
