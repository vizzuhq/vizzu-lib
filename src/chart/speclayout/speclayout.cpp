#include "speclayout.h"

#include "chart/generator/plot.h"

#include "bubblechartbuilder.h"
#include "tablechart.h"
#include "treemap.h"

namespace Vizzu::Gen
{

bool SpecLayout::addIfNeeded(Buckets &hierarchy) const
{
	auto &&options = plot.getOptions();
	if (options->getChannels().anyAxisSet()) return false;

	if (auto &markers = plot.getMarkers();
	    options->geometry == ShapeType::line
	    || options->geometry == ShapeType::area) {
		Charts::TableChart::setupVector(markers, true);
	}
	else if (options->getChannels().at(ChannelId::size).isEmpty()) {
		Charts::TableChart::setupVector(markers);
	}
	else {
		if (!plot.getDataCube().empty()) {
			auto &&[k, v] = plot.getDataCube().combinedSizeOf(
			    options->getChannels()
			        .at(ChannelId::size)
			        .dimensionIds);

			for (std::size_t i{},
			     size = std::min(k, hierarchy.size());
			     i < size;
			     ++i)
				hierarchy[i].resize(v);
			hierarchy.resize(k, std::vector<Marker *>(v));
		}
		else
			hierarchy.clear();

		for (auto &marker : markers)
			hierarchy[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    &marker;

		if (options->geometry == ShapeType::circle) {
			Charts::BubbleChartBuilder::setupVector(
			    *plot.getStyle().plot.marker.circleMaxRadius,
			    hierarchy);

			plot.keepAspectRatio = true;
		}
		else
			Charts::TreeMap::setupVector(hierarchy);
	}
	return true;
}
}
