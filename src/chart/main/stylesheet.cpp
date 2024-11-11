#include "stylesheet.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <map>
#include <numbers>
#include <string>
#include <vector>

#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"
#include "base/gfx/length.h"
#include "base/math/range.h"
#include "base/refl/auto_accessor.h"
#include "base/style/impl.tpp"
#include "chart/generator/plot.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/options.h"
#include "chart/options/shapetype.h"

#include "layout.h"

namespace Refl::Access
{
using Vizzu::Styles::Chart;
template <>
const std::map<std::string, Accessor<Chart, true>, std::less<>> &
getAccessors<Chart, true>()
{
	return Style::getAccessors<Chart>();
}
}

namespace Vizzu::Styles
{

Chart Sheet::getFullParams(const Gen::PlotOptionsPtr &options,
    const Geom::Size &size)
{
	this->options = options.get();

	calcDefaults(size);

	return Base::getFullParams();
}

void Sheet::calcDefaults(const Geom::Size &size)
{
	defaultParams = Chart::def();

	defaultParams.fontSize = Gfx::Length{baseFontSize(size, true)};

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
	if (options->coordSystem.get() == Gen::CoordSystem::polar) {
		defaultParams.plot.paddingLeft = Gfx::Length{0};
	}
	else if (!options->getChannels().anyAxisSet()) {
		defaultParams.plot.paddingLeft =
		    Gfx::Length::Emphemeral(45.0 / 12.0);
	}
	else if (options->getVerticalAxis().isDimension()) {
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
	auto &def = defaultParams.plot.xAxis.label;
	if (options->coordSystem.get() == Gen::CoordSystem::polar) {
		def.position = AxisLabel::Position::max_edge;
		def.side = AxisLabel::Side::positive;
	}
	else if (const auto &xAxis =
	             options->getChannels().at(Gen::AxisId::x);
	         xAxis.isDimension() && xAxis.hasDimension()
	         && options->angle == 0)
		def.angle.reset();
}

void Sheet::setAxisTitle()
{
	if (options->coordSystem.get() == Gen::CoordSystem::polar) {
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

	if (!options->getChannels().anyAxisSet()) {
		defaultParams.plot.marker.borderWidth = 0.5;
		defaultParams.plot.marker.borderOpacity = 0.7;
	}
	else {
		if (options->geometry == Gen::ShapeType::circle
		    && options->getChannels()
		           .at(Gen::ChannelId::size)
		           .isMeasure()
		    && (options->mainAxis().isMeasure()
		        || options->subAxis().isMeasure())) {
			defaultParams.plot.marker.borderWidth = 1;
			defaultParams.plot.marker.fillOpacity = 0.8;
		}
		else if (options->geometry == Gen::ShapeType::rectangle) {
			const auto &vertical = options->getVerticalAxis();
			const auto &horizontal = options->getHorizontalAxis();
			if (auto polar = options->coordSystem.get()
			              == Gen::CoordSystem::polar;
			    polar && vertical.isEmpty())
				defaultParams.plot.marker.rectangleSpacing = 0;
			else if (auto needRectangleSpacing =
			             vertical.isMeasure()
			                 != horizontal.isMeasure()
			             && (!polar || vertical.isDimension());
			         !needRectangleSpacing) {
				defaultParams.plot.marker.rectangleSpacing = 0;
				if (vertical.isDimension()
				    || horizontal.isDimension()) {
					defaultParams.plot.marker.borderWidth = 0.5;
					defaultParams.plot.marker.borderOpacity = 0.7;
				}
			}
		}
	}
}

void Sheet::setMarkerLabels()
{
	auto &def = defaultParams.plot.marker.label;

	if (options->getChannels().anyAxisSet()
	    && (options->geometry != Gen::ShapeType::rectangle
	        || !options->subAxis().hasDimension())) {
		if (options->geometry == Gen::ShapeType::circle)
			def.position = MarkerLabel::Position::right;
		else {
			def.position = options->isHorizontal()
			                 ? MarkerLabel::Position::top
			                 : MarkerLabel::Position::right;

			if (isConnecting(options->geometry.get())) {
				def.paddingBottom = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingLeft = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingTop = Gfx::Length::Emphemeral(8 / 11.0);
				def.paddingRight = Gfx::Length::Emphemeral(8 / 11.0);
			}
		}
	}
	else
		def.position = MarkerLabel::Position::center;

	if (const auto &activePos =
	        activeParams.get().plot.marker.label.position;
	    (activePos ? activePos : def.position)
	    == MarkerLabel::Position::center)
		def.filter = Gfx::ColorTransform::Lightness(0.7);
}

void Sheet::setData()
{
	defaultParams.plot.marker.circleMinRadius =
	    options->getChannels().at(Gen::ChannelId::size).isEmpty()
	        ? 0.0105
	        : 0.006;
}

void Sheet::setAfterStyles(Gen::Plot &plot, const Geom::Size &size)
{
	auto &style = plot.getStyle();
	style.setup();

	if (auto &xLabel = style.plot.xAxis.label; !xLabel.angle) {
		auto plotX = size.x;

		auto em = style.calculatedSize();
		if (plot.getOptions()->legend.get())
			plotX -= style.legend.computedWidth(plotX, em);

		plotX -= style.plot.toMargin({plotX, 0}, em).getSpace().x;

		auto font = Gfx::Font{xLabel};

		std::vector<Math::Range<double>> ranges;
		bool has_collision = false;
		for (const auto &[label, item] :
		    plot.axises.at(Gen::AxisId::x).dimension.getValues()) {

			if (!item.label.get()) continue;

			auto textBoundary =
			    Gfx::ICanvas::textBoundary(font, label.value);
			auto textXHalfMargin =
			    xLabel.toInvMargin(textBoundary, font.size)
			        .getSpace()
			        .x
			    / 2.0;
			auto xHalfSize =
			    (textBoundary.x + textXHalfMargin) / plotX / 2.0;

			auto rangeCenter = item.range.middle();

			auto next_range =
			    Math::Range<double>::Raw(rangeCenter - xHalfSize,
			        rangeCenter + xHalfSize);

			if (std::any_of(ranges.begin(),
			        ranges.end(),
			        [&next_range](const Math::Range<double> &other)
			        {
				        return other.includes(next_range);
			        })) {
				has_collision = true;
				break;
			}
			ranges.push_back(next_range);
		}

		xLabel.angle.emplace(has_collision * std::numbers::pi / 4);
	}
}

}