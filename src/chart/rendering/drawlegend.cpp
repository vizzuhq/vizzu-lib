#include "drawlegend.h"

#include "base/gfx/draw/roundedrect.h"
#include "chart/generator/plot.h"
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
	auto contentRect =
	    style.contentRect(legendLayout, rootStyle.calculatedSize());

	auto &&info = Info{
	    .canvas = canvas,
	    .contentRect = contentRect,
	    .type = channelType,
	    .weight = weight,
	    .itemHeight = style.label.getHeight(),
	    .titleHeight = style.title.getHeight(),
	    .markerSize = style.marker.size->get(contentRect.size.y,
	        style.label.calculatedSize()),
	    .measure = plot->axises.at(channelType).measure,
	    .dimension = plot->axises.at(channelType).dimension,
	};

	DrawBackground{{ctx()}}.draw(canvas,
	    legendLayout,
	    style,
	    *events.background,
	    Events::Targets::legend(channelType));

	canvas.save();
	canvas.setClipRect(contentRect);

	drawTitle(info);

	drawDimension(info);

	drawMeasure(info);

	canvas.restore();
}

void DrawLegend::drawTitle(const Info &info) const
{
	auto rect = info.contentRect;
	rect.size.y = info.titleHeight;
	plot->axises.at(info.type).common.title.visit(
	    [this,
	        &info,
	        &rect,
	        mul = std::max<double>(info.measureEnabled,
	            info.dimensionEnabled)](::Anim::InterpolateIndex,
	        const auto &title)
	    {
		    if (title.weight <= 0) return;

		    DrawLabel{{ctx()}}.draw(info.canvas,
		        Geom::TransformedRect::fromRect(rect),
		        title.value,
		        style.title,
		        *events.title,
		        Events::Targets::legendTitle(title.value, info.type),
		        {.alpha = title.weight * info.weight * mul});
	    });
}

void DrawLegend::drawDimension(const Info &info) const
{
	if (!info.dimensionEnabled) return;

	auto label = DrawLabel{{ctx()}};
	for (const auto &value : info.dimension) {
		if (value.second.weight <= 0) continue;

		auto itemRect =
		    getItemRect(info, value.second.range.getMin());

		if (itemRect.y().getMax() >= info.contentRect.y().getMax())
			continue;

		auto alpha = std::min(value.second.weight, info.weight);

		drawMarker(info,
		    value.second.categoryValue,
		    colorBuilder.render(value.second.colorBase) * alpha,
		    getMarkerRect(info, itemRect));

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
			            info.type),
			        {.alpha = std::min(alpha, weighted.weight)});
		    });
	}
}

Geom::Rect DrawLegend::getItemRect(const Info &info, double index)
{
	Geom::Rect res = info.contentRect;
	res.pos.y += info.titleHeight + index * info.itemHeight;
	res.size.y = info.itemHeight;
	if (res.size.x < 0) res.size.x = 0;
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
	res.size.x = std::max(0.0, res.size.x - info.markerSize);
	return Geom::TransformedRect::fromRect(res);
}

void DrawLegend::drawMarker(const Info &info,
    std::string_view categoryValue,
    const Gfx::Color &color,
    const Geom::Rect &rect) const
{
	info.canvas.save();

	info.canvas.setBrushColor(color);
	info.canvas.setLineColor(color);
	info.canvas.setLineWidth(0);

	auto radius = rootStyle.legend.marker.type->factor<double>(
	                  Styles::Legend::Marker::Type::circle)
	            * rect.size.minSize() / 2.0;

	auto markerElement =
	    Events::Targets::legendMarker(info.dimension.category,
	        categoryValue,
	        info.type);

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
	switch (info.type) {
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
	    Events::Targets::measLegendLabel(text, info.type),
	    {.alpha = info.measureWeight * plusWeight});
}

Geom::Rect DrawLegend::getBarRect(const Info &info)
{
	Geom::Rect res = info.contentRect;
	res.pos.y += info.titleHeight + info.itemHeight / 2.0;
	res.size.y = 5 * info.itemHeight;
	res.size.x = info.markerSize;
	return res;
}

void DrawLegend::colorBar(const Info &info,
    const Geom::Rect &rect) const
{
	info.canvas.save();

	info.canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{*rootStyle.plot.marker.colorGradient
	                       * info.measureWeight});
	info.canvas.setLineColor(Gfx::Color::Transparent());
	info.canvas.setLineWidth(0);

	auto barElement =
	    Events::Targets::legendBar(Gen::ChannelId::color);

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
	Gfx::ColorGradient gradient;

	gradient.stops.emplace_back(0.0,
	    colorBuilder.render(Gen::ColorBase(0U, 0.0)));
	gradient.stops.emplace_back(0.5,
	    colorBuilder.render(Gen::ColorBase(0U, 0.5)));
	gradient.stops.emplace_back(1.0,
	    colorBuilder.render(Gen::ColorBase(0U, 1.0)));

	info.canvas.save();

	info.canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{gradient * info.measureWeight});
	info.canvas.setLineColor(Gfx::Color::Transparent());
	info.canvas.setLineWidth(0);

	auto barElement =
	    Events::Targets::legendBar(Gen::ChannelId::lightness);

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

	auto barElement =
	    Events::Targets::legendBar(Gen::ChannelId::size);

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