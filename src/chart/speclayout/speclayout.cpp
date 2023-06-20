#include "speclayout.h"

#include "bubblechartbuilder.h"
#include "tablechart.h"
#include "treemap.h"

namespace Vizzu::Gen
{

bool SpecLayout::addIfNeeded()
{
	using namespace Vizzu::Charts;

	auto options = plot.getOptions();
	auto &markers = plot.getMarkers();
	auto &style = plot.getStyle();

	if (options->getChannels().anyAxisSet()) return false;

	if (options->shapeType == ShapeType::Line
	    || options->shapeType == ShapeType::Area) {
		TableChart::setupVector(markers, true);
	}
	else if (options->getChannels().at(ChannelId::size).isEmpty()) {
		TableChart::setupVector(markers);
	}
	else {
		Plot::Buckets hierarchy;
		for (auto i = 0u; i < markers.size(); i++) {
			auto &marker = markers[i];
			hierarchy[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    i;
		}
		if (options->shapeType == ShapeType::Circle) {
			BubbleChartBuilder::setupVector(markers,
			    *style.plot.marker.circleMaxRadius,
			    hierarchy);

			plot.keepAspectRatio = true;
		}
		else if (options->shapeType == ShapeType::Rectangle) {
			TreeMap::setupVector(markers, hierarchy);
		}
		else
			return false;
	}
	return true;
}
}
