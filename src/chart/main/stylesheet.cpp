#include "stylesheet.h"

#include <cmath>
#include "base/io/log.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

Chart Sheet::getFullParams(
	const Diag::DiagramOptionsPtr &options,
	const Geom::Size &size)
{
	this->options = options.get();

	calcDefaults(size);

	return Base::getFullParams();
}

void Sheet::calcDefaults(const Geom::Size &size)
{
	defaultParams = Chart::def();

	defaultParams.fontSize = baseFontSize(size, true);

	setAxis();
	setMarkers();
	setData();
}

double Sheet::nominalSize(const Geom::Size &size)
{
	// empirical formula
	return pow(fabs(size.x), 0.25)*pow(fabs(size.y), 0.75);
}

double Sheet::baseFontSize(const Geom::Size &size, bool rounded)
{
	// approximated with proportional rate growth exponential function
	// using empirical values
	auto Y0 = 1.4;
	auto V0 = -0.03;
	auto K = 0.00175;

	auto x = nominalSize(size);
	auto fontSize = Y0 - (V0/K) * (1-exp(-K * x));

	if (!rounded) return fontSize;

	return fontSize >= 9 
		? round(fontSize) 
		: 0.5 * round(fontSize * 2.0);
}

void Sheet::setAxis()
{
	setAxisLabels();
	setAxisTitle();
}

void Sheet::setAxisLabels()
{
	if (options->polar.get())
	{
		auto &def = defaultParams.plot.xAxis.label;
		def.position = AxisLabel::Position::max_edge;
		def.side = AxisLabel::Side::positive;
	}
}

void Sheet::setAxisTitle()
{
	if (options->polar.get())
	{
		auto &def = defaultParams.plot.xAxis.title;
		def.position = AxisTitle::Position::max_edge;
		def.side = AxisTitle::Side::negative;
		def.paddingTop = Gfx::Length::Emphemeral(25.0/14.0);
		defaultParams.plot.yAxis.title.paddingBottom = 
			Gfx::Length::Emphemeral(25.0/14.0);
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
	auto &def = defaultParams.plot.marker.label;

	if (options->getScales().anyAxisSet()
		&& !(
				options->shapeType.get() == Diag::ShapeType::Type::Rectangle
				&& options->subAxis().discreteCount() > 0
			)
		)
	{
		if (options->shapeType.get() == Diag::ShapeType::Type::Circle)
		{
			def.position = MarkerLabel::Position::right;
		}
		else
		{
			def.position = options->horizontal.get() 
				? MarkerLabel::Position::top
				: MarkerLabel::Position::right;
		}
	} else {
		def.position = MarkerLabel::Position::center;
	}

	const auto &pos =
		(activeParams && activeParams->plot.marker.label.position)
		? activeParams->plot.marker.label.position
		: def.position;

	if (pos == MarkerLabel::Position::center)
	{
		def.filter = Gfx::ColorTransform::Lightness(0.7);
	}
}

void Sheet::setData()
{
	defaultParams.data.circleMinRadius = 
		options->getScales().at(Diag::ScaleId::size).isEmpty()
		? 7 : 4;
}
