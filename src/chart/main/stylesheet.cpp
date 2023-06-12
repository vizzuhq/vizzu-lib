#include "stylesheet.h"

#include <cmath>

#include "base/io/log.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

Chart Sheet::getFullParams(const Diag::PlotOptionsPtr &options,
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

	setPlot();
	setAxis();
	setMarkers();
	setData();
}

double Sheet::nominalSize(const Geom::Size &size)
{
	// empirical formula
	return pow(fabs(size.x), 0.25) * pow(fabs(size.y), 0.75);
}

double Sheet::baseFontSize(const Geom::Size &size, bool rounded)
{
	// approximated with proportional rate growth exponential function
	// using empirical values
	auto Y0 = 2;
	auto V0 = -0.031;
	auto K = 0.002;

	auto x = nominalSize(size);
	auto fontSize = Y0 - (V0 / K) * (1 - exp(-K * x));

	if (!rounded) return fontSize;

	return fontSize >= 10 ? round(fontSize)
	                      : 0.5 * ceil(fontSize * 2.0);
}

void Sheet::setPlot()
{
	if ((bool)options->polar.get()) {
		defaultParams.plot.paddingLeft = 0;
	}
	else if (!options->getScales().anyAxisSet()) {
		defaultParams.plot.paddingLeft =
		    Gfx::Length::Emphemeral(45.0 / 12.0);
	}
	else if (options->getVeritalAxis().isPseudoDiscrete()) {
		defaultParams.plot.paddingLeft =
		    Gfx::Length::Emphemeral(80.0 / 12.0);
	}
	else {
		defaultParams.plot.paddingLeft =
		    Gfx::Length::Emphemeral(50.0 / 12.0);
	}
}

void Sheet::setAxis()
{
	setAxisLabels();
	setAxisTitle();
}

void Sheet::setAxisLabels()
{
	if (options->polar.get()) {
		auto &def = defaultParams.plot.xAxis.label;
		def.position = AxisLabel::Position::max_edge;
		def.side = AxisLabel::Side::positive;
	}
}

void Sheet::setAxisTitle()
{
	if (options->polar.get()) {
		auto &def = defaultParams.plot.xAxis.title;
		def.position = AxisTitle::Position::max_edge;
		def.side = AxisTitle::Side::positive;
		def.paddingTop = Gfx::Length::Emphemeral(25.0 / 14.0);
		defaultParams.plot.yAxis.title.paddingBottom =
		    Gfx::Length::Emphemeral(25.0 / 14.0);
	}
}

void Sheet::setMarkers()
{
	setMarkerLabels();

	if (!options->getScales().anyAxisSet()) {
		defaultParams.plot.marker.borderWidth = 0.5;
		defaultParams.plot.marker.borderOpacity = 0.7;
	}

	if (options->getScales().anyAxisSet()
	    && options->shapeType.get() == Diag::ShapeType::Type::Circle
	    && !options->getScales()
	            .at(Diag::ScaleId::size)
	            .isPseudoDiscrete()
	    && (!options->mainAxis().isPseudoDiscrete()
	        || !options->subAxis().isPseudoDiscrete())) {
		defaultParams.plot.marker.borderWidth = 1;
		defaultParams.plot.marker.fillOpacity = 0.8;
	}

	if (options->getScales().anyAxisSet()
	    && options->shapeType.get()
	           == Diag::ShapeType::Type::Rectangle
	    && (bool)options->polar.get()
	    && options->getVeritalAxis().isEmpty()) {
		defaultParams.plot.marker.rectangleSpacing = 0;
	}
}

void Sheet::setMarkerLabels()
{
	auto &def = defaultParams.plot.marker.label;

	if (options->getScales().anyAxisSet()
	    && !(options->shapeType.get()
	             == Diag::ShapeType::Type::Rectangle
	         && options->subAxis().discreteCount() > 0)) {
		if (options->shapeType.get()
		    == Diag::ShapeType::Type::Circle) {
			def.position = MarkerLabel::Position::right;
		}
		else {
			def.position = options->horizontal.get()
			                 ? MarkerLabel::Position::top
			                 : MarkerLabel::Position::right;

			if (options->shapeType.get()
			        == Diag::ShapeType::Type::Area
			    || options->shapeType.get()
			           == Diag::ShapeType::Type::Line) {
				def.paddingBottom = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingLeft = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingTop = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingRight = Gfx::Length::Emphemeral(8 / 11.0);
			}
		}
	}
	else {
		def.position = MarkerLabel::Position::center;
	}

	const auto &pos =
	    (activeParams && activeParams->plot.marker.label.position)
	        ? activeParams->plot.marker.label.position
	        : def.position;

	if (pos == MarkerLabel::Position::center) {
		def.filter = Gfx::ColorTransform::Lightness(0.7);
	}
}

void Sheet::setData()
{
	defaultParams.plot.marker.circleMinRadius =
	    options->getScales().at(Diag::ScaleId::size).isEmpty()
	        ? 0.0105
	        : 0.006;
}
