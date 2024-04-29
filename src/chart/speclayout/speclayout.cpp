#include "speclayout.h"

#include "chart/generator/plot.h"

#include "bubblechartbuilder.h"
#include "tablechart.h"
#include "treemap.h"

namespace Vizzu::Gen
{

bool SpecLayout::addIfNeeded()
{

	auto options = plot.getOptions();
	auto &markers = plot.getMarkers();
	auto &style = plot.getStyle();

	if (options->getChannels().anyAxisSet()) return false;

	if (options->geometry == ShapeType::line
	    || options->geometry == ShapeType::area) {
		Charts::TableChart::setupVector(markers, true);
	}
	else if (options->getChannels().at(ChannelId::size).isEmpty()) {
		Charts::TableChart::setupVector(markers);
	}
	else {
		Plot::Buckets hierarchy;
		if (!plot.getDataCube().empty()) {
			auto &&[k, v] = plot.getDataCube().combinedSizeOf(
			    options->getChannels()
			        .at(ChannelId::size)
			        .dimensionIds);
			hierarchy.resize(k, std::vector<uint64_t>(v));
		}
		for (auto i = 0U; i < markers.size(); ++i) {
			auto &marker = markers[i];
			hierarchy[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    i;
		}
		if (options->geometry == ShapeType::circle) {
			Charts::BubbleChartBuilder::setupVector(markers,
			    *style.plot.marker.circleMaxRadius,
			    hierarchy);

			plot.keepAspectRatio = true;
		}
		else if (options->geometry == ShapeType::rectangle) {
			Charts::TreeMap::setupVector(markers, hierarchy);
		}
		else
			return false;
	}
	return true;
}
}
