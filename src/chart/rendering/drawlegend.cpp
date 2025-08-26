#include "drawlegend.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
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
#include "base/math/floating.h"
#include "base/math/fuzzybool.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "base/text/smartstring.h"
#include "chart/generator/axis.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"
#include "chart/rendering/colorbuilder.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"
#include "dataframe/old/types.h"

namespace Vizzu::Draw
{

void DrawLegend::draw(Gfx::ICanvas &canvas,
    const Geom::Rect &legendLayout,
    Gen::LegendId channelType,
    double weight,
    const Gen::Axis &axis) const
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
	    .axis = axis,
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
	    style.translateY->get(yOverflow, info.itemHeight) + range.min;

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
	info.axis.title.visit(
	    [this,
	        &info,
	        &rect = info.titleRect,
	        mul = Math::FuzzyBool::Or(info.measureEnabled,
	            !info.axis.dimension.empty())](
	        ::Anim::InterpolateIndex,
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
	for (auto label = DrawLabel{{ctx()}}; const auto &[sindex, item] :
	     info.axis.dimension.getValues()) {
		auto weight = item.weight(info.axis.dimension.factor);
		if (weight <= 0) continue;

		double pos{};

		auto &&start = item.startPos;
		auto &&end = item.endPos;
		if (start && end)
			pos = Math::Niebloid::interpolate(*start,
			    *end,
			    info.axis.dimension.factor);
		else if (start)
			pos = *start;
		else
			pos = *end;

		auto itemRect = getItemRect(info, pos);

		if (itemRect.y().min > info.markerWindowRect.y().max
		    || itemRect.y().max < info.markerWindowRect.y().min)
			continue;

		const auto needGradient =
		    itemRect.y().max
		        > info.markerWindowRect.y().max - info.fadeHeight
		    || itemRect.y().min
		           < info.markerWindowRect.y().min + info.fadeHeight;

		const auto alpha = Math::FuzzyBool::And(weight, info.weight);

		drawMarker(info,
		    sindex,
		    colorBuilder.render(item.colorBase) * alpha,
		    getMarkerRect(info, itemRect),
		    needGradient);

		auto &&value = Gen::DimensionAxis::mergedLabels(sindex);

		label.draw(info.canvas,
		    getLabelRect(info, itemRect),
		    value,
		    style.label,
		    *events.label,
		    Events::Targets::dimLegendLabel(sindex,
		        value,
		        info.properties),
		    {.colorTransform = Gfx::ColorTransform::Opacity(alpha),
		        .gradient =
		            needGradient
		                ? std::ref(info.fadeBarGradient)
		                : decltype(DrawLabel::Options::gradient){}});
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
    const std::vector<Data::SliceIndex> &sindex,
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

	if (auto &&markerElement{
	        Events::Targets::legendMarker(sindex, info.properties)};
	    events.marker->invoke(
	        Events::OnRectDrawEvent(*markerElement, {rect, false}))) {
		auto radius = rootStyle.legend.marker.type->factor(
		                  Styles::Legend::Marker::Type::circle)
		            * rect.size.minSize() / 2.0;

		Gfx::Draw::RoundedRect(info.canvas, rect, radius);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(markerElement));
	}

	info.canvas.restore();
}

void DrawLegend::drawMeasure(const Info &info) const
{
	if (info.measureEnabled <= 0) return;

	const auto &[unit, weight] =
	    info.axis.measure.unit.get_or_first(::Anim::first);

	extremaLabel(info, info.axis.measure.range.max, unit, 0, weight);
	extremaLabel(info, info.axis.measure.range.min, unit, 5, weight);

	auto bar = getBarRect(info);

	using ST = Gen::LegendId;
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

Math::Range<> DrawLegend::markersLegendRange(const Info &info)
{
	auto res = Math::Range<>{{}, {}};

	const std::uint32_t measMax =
	    Math::Floating::is_zero(info.measureEnabled) ? 0 : 6;

	std::uint32_t startMax{};
	std::uint32_t endMax{};
	for (const auto &item : info.axis.dimension) {
		if (auto &&start = item.startPos)
			startMax = std::max(startMax, *start + 1);
		if (auto &&end = item.endPos)
			endMax = std::max(endMax, *end + 1);
	}

	if (auto &&[min, max] = std::minmax(startMax, endMax); !min)
		res.include(Math::Niebloid::interpolate(measMax,
		    max,
		    Math::FuzzyBool::And<double>(1 - info.measureEnabled,
		        info.axis.dimension.factor)));
	else
		res.include(Math::Niebloid::interpolate(startMax,
		                endMax,
		                info.axis.dimension.factor)
		            * info.itemHeight);
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