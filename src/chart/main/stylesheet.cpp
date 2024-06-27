#include "stylesheet.h"

#include <cmath>

#include "base/style/impl.tpp"
#include "chart/generator/plot.h"

#include "layout.h"

template Style::ParamRegistry<Vizzu::Styles::Chart>::ParamRegistry();

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
	else if (options->getVeritalAxis().isDimension()) {
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
	             options->getChannels().at(Gen::ChannelId::x);
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
		if (options->geometry == Gen::ShapeType::rectangle
		    && options->getChannels().getMeasures().empty())
			defaultParams.plot.marker.rectangleSpacing = 0;
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

		if (options->geometry == Gen::ShapeType::rectangle
		    && options->coordSystem.get()
		           == Gen::CoordSystem::polar) {
			if (options->getHorizontalAxis().isDimension()
			    && options->getHorizontalAxis().hasDimension()
			    && options->getVeritalAxis().isMeasure()) {
				defaultParams.plot.marker.rectangleSpacing = 0;
				defaultParams.plot.marker.borderWidth = 0.5;
				defaultParams.plot.marker.borderOpacity = 0.7;
			}
			else if (options->getVeritalAxis().isEmpty())
				defaultParams.plot.marker.rectangleSpacing = 0;
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

			if (options->geometry == Gen::ShapeType::area
			    || options->geometry == Gen::ShapeType::line) {
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
		for (const auto &pair :
		    plot.axises.at(Gen::ChannelId::x).dimension) {

			if (pair.second.weight == 0) continue;

			auto textBoundary = Gfx::ICanvas::textBoundary(font,
			    pair.second.label.get());
			auto textXHalfMargin =
			    xLabel.toInvMargin(textBoundary, font.size)
			        .getSpace()
			        .x
			    / 2.0;
			auto xHalfSize =
			    (textBoundary.x + textXHalfMargin) / plotX / 2.0;

			auto rangeCenter = pair.second.range.middle();

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

		xLabel.angle.emplace(has_collision * M_PI / 4);
	}
}

}