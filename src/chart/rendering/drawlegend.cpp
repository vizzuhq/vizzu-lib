#include "drawlegend.h"

#include <cmath>
#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/affinetransform.h"
#include "base/geom/rect.h"
#include "base/geom/transformedrect.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/draw/roundedrect.h"
#include "base/gfx/lineargradient.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "base/text/smartstring.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"
#include "chart/rendering/colorbuilder.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"

namespace Vizzu::Draw
{

void DrawLegend::draw(Gfx::ICanvas &canvas,
    const Geom::Rect &legendLayout,
    Gen::ChannelId channelType,
    double weight) const
{
	auto markerWindowRect =
	    style.contentRect(legendLayout, rootStyle.calculatedSize());

	auto titleRect =
	    markerWindowRect.popBottom(style.title.getHeight());
	auto markerWindowHeight = markerWindowRect.height();
	auto itemHeight = style.label.getHeight();
	auto markerSize = style.marker.size->get(itemHeight,
	    style.label.calculatedSize());

	auto fadeHeight = markerSize;
	markerWindowRect =
	    markerWindowRect
	    + Geom::Rect{{0, -fadeHeight}, {0, 2 * fadeHeight}};

	auto fadeElementPercent = fadeHeight / markerWindowRect.height();

	auto &&info = Info{.canvas = canvas,
	    .titleRect = titleRect,
	    .markerWindowRect = markerWindowRect,
	    .fadeHeight = fadeHeight,
	    .weight = weight,
	    .itemHeight = itemHeight,
	    .markerSize = markerSize,
	    .measure = plot->axises.at(channelType).measure,
	    .dimension = plot->axises.at(channelType).dimension,
	    .properties = {.channel = channelType},
	    .fadeBarGradient = {markerWindowRect.leftSide(),
	        {.line = {},
	            .colors = {{
	                {0.0, {}},
	                {fadeElementPercent, {}},
	                {1.0 - fadeElementPercent, {}},
	                {1.0, {}},
	            }}}}};

	auto &&range = markersLegendRange(info);
	info.properties.scrollHeight = range.size();
	auto yOverflow =
	    info.properties.scrollHeight - markerWindowHeight;
	if (std::signbit(yOverflow)) yOverflow = 0.0;
	info.properties.scrollTop =
	    style.translateY->get(yOverflow, info.itemHeight)
	    + range.getMin();

	DrawBackground{{ctx()}}.draw(canvas,
	    legendLayout,
	    style,
	    *events.background,
	    Events::Targets::legend(info.properties));

	canvas.save();

	canvas.setClipRect(markerWindowRect);

	drawDimension(info);

	drawMeasure(info);

	canvas.restore();

	canvas.save();

	canvas.setClipRect(titleRect);

	drawTitle(info);

	canvas.restore();
}

const Gfx::LinearGradient &DrawLegend::FadeBarGradient::operator()(
    const Gfx::Color &color,
    const Geom::AffineTransform &transform)
{
	for (auto &stop : gradient.colors.stops) stop.value = color;

	gradient.colors.stops.front().value.alpha = 0.0;
	gradient.colors.stops.back().value.alpha = 0.0;

	gradient.line = transform(origLine);

	return gradient;
}

void DrawLegend::drawTitle(const Info &info) const
{
	plot->axises.at(info.properties.channel)
	    .common.title.visit(
	        [this,
	            &info,
	            &rect = info.titleRect,
	            mul = Math::FuzzyBool::Or(info.measureEnabled,
	                info.dimensionEnabled)](::Anim::InterpolateIndex,
	            const auto &title)
	        {
		        if (title.weight <= 0) return;

		        DrawLabel{{ctx()}}.draw(info.canvas,
		            Geom::TransformedRect::fromRect(rect),
		            title.value,
		            style.title,
		            *events.title,
		            Events::Targets::legendTitle(title.value,
		                info.properties),
		            {.colorTransform = Gfx::ColorTransform::Opacity(
		                 Math::FuzzyBool::And(title.weight,
		                     info.weight,
		                     mul))});
	        });
}

void DrawLegend::drawDimension(Info &info) const
{
	if (!info.dimensionEnabled) return;

	auto label = DrawLabel{{ctx()}};
	for (const auto &value : info.dimension) {
		if (value.second.weight <= 0) continue;

		auto itemRect =
		    getItemRect(info, value.second.range.getMin());

		if (itemRect.y().getMin() > info.markerWindowRect.y().getMax()
		    || itemRect.y().getMax()
		           < info.markerWindowRect.y().getMin())
			continue;

		const auto needGradient =
		    itemRect.y().getMax()
		        > info.markerWindowRect.y().getMax() - info.fadeHeight
		    || itemRect.y().getMin()
		           < info.markerWindowRect.y().getMin()
		                 + info.fadeHeight;

		const auto alpha =
		    Math::FuzzyBool::And(value.second.weight, info.weight);

		drawMarker(info,
		    value.second.categoryValue,
		    colorBuilder.render(value.second.colorBase) * alpha,
		    getMarkerRect(info, itemRect),
		    needGradient);

		value.second.label.visit(
		    [&](::Anim::InterpolateIndex, const auto &weighted)
		    {
			    label.draw(info.canvas,
			        getLabelRect(info, itemRect),
			        weighted.value,
			        style.label,
			        *events.label,
			        Events::Targets::dimLegendLabel(
			            info.dimension.category,
			            value.second.categoryValue,
			            value.second.categoryValue,
			            info.properties),
			        {.colorTransform = Gfx::ColorTransform::Opacity(
			             Math::FuzzyBool::And(alpha,
			                 weighted.weight)),
			            .gradient = needGradient
			                          ? std::ref(info.fadeBarGradient)
			                          : decltype(DrawLabel::Options::
			                                  gradient){}});
		    });
	}
}

Geom::Rect DrawLegend::getItemRect(const Info &info, double index)
{
	Geom::Rect res = info.markerWindowRect;
	res.pos.y += info.fadeHeight + index * info.itemHeight
	           - info.properties.scrollTop;
	res.size.y = info.itemHeight;
	if (std::signbit(res.size.x)) res.size.x = 0;
	return res;
}

Geom::Rect DrawLegend::getMarkerRect(const Info &info,
    const Geom::Rect &itemRect)
{
	Geom::Rect res = itemRect;
	res.pos.y += info.itemHeight / 2.0 - info.markerSize / 2.0;
	res.size = Geom::Size::Square(info.markerSize);
	return res;
}

Geom::TransformedRect DrawLegend::getLabelRect(const Info &info,
    const Geom::Rect &itemRect)
{
	Geom::Rect res = itemRect;
	res.pos.x += info.markerSize;
	res.size.x -= info.markerSize;
	if (std::signbit(res.size.x)) res.size.x = 0.0;
	return Geom::TransformedRect::fromRect(res);
}

void DrawLegend::drawMarker(Info &info,
    std::string_view categoryValue,
    const Gfx::Color &color,
    const Geom::Rect &rect,
    bool needGradient) const
{
	info.canvas.save();

	if (needGradient)
		info.canvas.setBrushGradient(info.fadeBarGradient(color));
	else
		info.canvas.setBrushColor(color);

	info.canvas.setLineColor(color);
	info.canvas.setLineWidth(0);

	auto radius = rootStyle.legend.marker.type->factor(
	                  Styles::Legend::Marker::Type::circle)
	            * rect.size.minSize() / 2.0;

	auto markerElement =
	    Events::Targets::legendMarker(info.dimension.category,
	        categoryValue,
	        info.properties);

	if (events.marker->invoke(
	        Events::OnRectDrawEvent(*markerElement, {rect, false}))) {
		Gfx::Draw::RoundedRect(info.canvas, rect, radius);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(markerElement));
	}

	info.canvas.restore();
}

void DrawLegend::drawMeasure(const Info &info) const
{
	if (info.measureEnabled <= 0) return;

	info.measure.unit.visit(
	    [this, &info](::Anim::InterpolateIndex, const auto &unit)
	    {
		    extremaLabel(info,
		        info.measure.range.getMax(),
		        unit.value,
		        0,
		        unit.weight);
		    extremaLabel(info,
		        info.measure.range.getMin(),
		        unit.value,
		        5,
		        unit.weight);
	    });

	auto bar = getBarRect(info);

	using ST = Gen::ChannelId;
	switch (info.properties.channel) {
	case ST::color: colorBar(info, bar); break;
	case ST::lightness: lightnessBar(info, bar); break;
	case ST::size: sizeBar(info, bar); break;
	default: break;
	}
}

void DrawLegend::extremaLabel(const Info &info,
    double value,
    const std::string &unit,
    int pos,
    double plusWeight) const
{
	auto text = Text::SmartString::fromPhysicalValue(value,
	    *style.label.numberFormat,
	    static_cast<size_t>(*style.label.maxFractionDigits),
	    *style.label.numberScale,
	    unit);

	DrawLabel{{ctx()}}.draw(info.canvas,
	    getLabelRect(info, getItemRect(info, pos)),
	    text,
	    style.label,
	    *events.label,
	    Events::Targets::measLegendLabel(text, info.properties),
	    {.colorTransform = Gfx::ColorTransform::Opacity(
	         Math::FuzzyBool::And(info.measureWeight, plusWeight))});
}

Geom::Rect DrawLegend::getBarRect(const Info &info)
{
	Geom::Rect res = info.markerWindowRect;
	res.pos.y += info.fadeHeight + info.itemHeight / 2.0;
	res.size.y = 5 * info.itemHeight;
	res.size.x = info.markerSize;
	return res;
}

Math::Range<double> DrawLegend::markersLegendRange(const Info &info)
{
	Math::Range<double> res;
	if (info.measureEnabled > 0.0) {
		res.include(0.0);
		res.include(6.0 * info.itemHeight);
	}

	if (info.dimensionEnabled)
		for (const auto &value : info.dimension)
			res.include({value.second.range.getMin()
			                 * info.itemHeight,
			    (value.second.range.getMax() + 1) * info.itemHeight});

	return res;
}

void DrawLegend::colorBar(const Info &info,
    const Geom::Rect &rect) const
{
	info.canvas.save();

	info.canvas.setBrushGradient({rect.leftSide(),
	    *rootStyle.plot.marker.colorGradient * info.measureWeight});
	info.canvas.setLineColor(Gfx::Color::Transparent());
	info.canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(info.properties);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		info.canvas.rectangle(rect);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	info.canvas.restore();
}

void DrawLegend::lightnessBar(const Info &info,
    const Geom::Rect &rect) const
{
	info.canvas.save();

	info.canvas.setBrushGradient({rect.leftSide(),
	    {{0.0,
	         colorBuilder.render(Gen::ColorBase(0U, 0.0))
	             * info.measureWeight},
	        {0.5,
	            colorBuilder.render(Gen::ColorBase(0U, 0.5))
	                * info.measureWeight},
	        {1.0,
	            colorBuilder.render(Gen::ColorBase(0U, 1.0))
	                * info.measureWeight}}});
	info.canvas.setLineColor(Gfx::Color::Transparent());
	info.canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(info.properties);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		info.canvas.rectangle(rect);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	info.canvas.restore();
}

void DrawLegend::sizeBar(const Info &info,
    const Geom::Rect &rect) const
{
	info.canvas.save();

	info.canvas.setBrushColor(
	    Gfx::Color::Gray(0.8) * info.measureWeight);
	info.canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(info.properties);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		info.canvas.beginPolygon();
		info.canvas.addPoint(rect.bottomLeft());
		info.canvas.addPoint(rect.bottomRight());
		info.canvas.addPoint(rect.topSide().center());
		info.canvas.endPolygon();
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	info.canvas.restore();
}

}