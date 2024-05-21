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
    markerId(marker.idx),
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
	return source.markers.size() != target.markers.size()
	    || source.markersInfo != target.markersInfo;
}
void Plot::mergeMarkersWithCellInfo(Plot &source, Plot &target)
{
	auto &&smarkers = source.markers;
	auto &&tmarkers = target.markers;
	auto &&ssize = smarkers.size();
	auto &&tsize = tmarkers.size();
	for (auto i = ssize; i < tsize; ++i)
		smarkers.emplace_back(tmarkers[i]).enabled = false;

	for (auto i = tsize; i < ssize; ++i)
		tmarkers.emplace_back(smarkers[i]).enabled = false;

	const auto markers_size = std::min(ssize, tsize);
	for (std::size_t ix{}; ix < markers_size; ++ix)
		if (auto &scell = source.markers[ix].cellInfo,
		    &tcell = target.markers[ix].cellInfo;
		    scell && !tcell)
			tcell = scell;
		else if (!scell && tcell)
			scell = tcell;
}

}
