#include "speclayout.h"

#include "bubblechartbuilder.h"
#include "tablechart.h"
#include "treemap.h"

namespace Vizzu::Diag
{

bool SpecLayout::addIfNeeded()
{
	using namespace Vizzu::Charts;

	auto options = diagram.getOptions();
	auto &markers = diagram.getMarkers();
	auto &style = diagram.getStyle();

	if (options->getScales().anyAxisSet()) return false;

	if (options->shapeType.get() == ShapeType::Line
	    || options->shapeType.get() == ShapeType::Area)
	{
		TableChart::setupVector(markers, true);
	}
	else if (!options->getScales().anyScaleOfType(Scale::Size))
	{
		TableChart::setupVector(markers);
	}
	else
	{
		Diagram::Buckets hierarchy;
		for (auto i = 0u; i < markers.size(); i++)
		{
			auto &marker = markers[i];
			hierarchy[marker.sizeId.seriesId][marker.sizeId.itemId] =
			    i;
		}
		if (options->shapeType.get() == ShapeType::Circle)
		{
			if (options->bubbleChartAlgorithm.get()
			    == BubbleChartAlgorithm::slow)
			{
				BubbleChartBuilder<BubbleChartV1>::setupVector(
				    markers,
				    *style.data.circleMaxRadius,
				    options->alignType.get() == Base::Align::Fit
				        ? Boundary::Box
				        : Boundary::Circular,
				    hierarchy);
			}
			else
			{
				BubbleChartBuilder<BubbleChartV2>::setupVector(
				    markers,
				    *style.data.circleMaxRadius,
				    options->alignType.get() == Base::Align::Fit
				        ? Boundary::Box
				        : Boundary::Circular,
				    hierarchy);
			}
			diagram.keepAspectRatio = true;
		}
		else if (options->shapeType.get() == ShapeType::Rectangle)
		{
			TreeMap::setupVector(markers, hierarchy);
		}
		else
			return false;
	}
	return true;
}
}
