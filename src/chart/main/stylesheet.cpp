#include "stylesheet.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

Chart Sheet::getFullParams(const Diag::DiagramOptionsPtr &options)
{
	this->options = options.get();

	calcDefaults();

	return Base::getFullParams();
}

void Sheet::calcDefaults()
{
	setAxis();
	setMarkers();
	setData();
}

void Sheet::setAxis()
{
	if (options->polar.get())
	{
		defaultParams.plot.xAxis.label.position =
			AxisLabel::Position::max_edge;

		defaultParams.plot.xAxis.label.side =
			AxisLabel::Side::positive;
	}
	else
	{
		defaultParams.plot.xAxis.label.position =
			AxisLabel::Position::min_edge;

		defaultParams.plot.xAxis.label.side =
			AxisLabel::Side::negative;
	}
}

void Sheet::setMarkers()
{
	setMarkerLabels();

	if (!options->getScales().anyAxisSet() && 
		options->shapeType.get() == Diag::ShapeType::Type::Rectangle)
	{
		defaultParams.plot.marker.borderWidth = 0.5;
		defaultParams.plot.marker.borderOpacity = 0.7;
	}

}

void Sheet::setMarkerLabels()
{
	if (options->shapeType.get() == Diag::ShapeType::Type::Rectangle
		&& options->getScales().bothAxisSet())
	{

	}

	const auto &pos =
		(activeParams && activeParams->plot.marker.label.position)
		? activeParams->plot.marker.label.position
		: defaultParams.plot.marker.label.position;

	if (pos == MarkerLabel::Position::center)
	{
		defaultParams.plot.marker.label.filter = 
			Gfx::ColorTransform::Lightness(0.7);
	}
}

void Sheet::setData()
{
	defaultParams.data.circleMinRadius = 
		options->getScales().at(Diag::Scale::Type::Size).isEmpty()
		? 7 : 4;
}
