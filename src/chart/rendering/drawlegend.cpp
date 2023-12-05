#include "drawlegend.h"

#include "base/gfx/draw/roundedrect.h"
#include "chart/rendering/colorbuilder.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"

namespace Vizzu::Draw
{

DrawLegend::DrawLegend(const DrawingContext &context,
    Gen::ChannelId channelType,
    double weight) :
    DrawingContext(context),
    colorBuilder(context.rootStyle.plot.marker.lightnessRange(),
        *context.rootStyle.plot.marker.colorPalette,
        *context.rootStyle.plot.marker.colorGradient),
    events(context.rootEvents.draw.legend),
    style(context.rootStyle.legend),
    type(channelType),
    weight(weight),
    itemHeight(DrawLabel::getHeight(style.label, canvas)),
    titleHeight(DrawLabel::getHeight(style.title, canvas))
{

	contentRect = style.contentRect(layout.legend,
	    context.rootStyle.calculatedSize());

	DrawBackground(*this,
	    layout.legend,
	    style,
	    events.background,
	    Events::Targets::legend(channelType));

	if (static_cast<std::size_t>(type)
	    < std::size(plot->measureAxises.axises)) {
		canvas.save();
		canvas.setClipRect(contentRect);

		const auto measureAxis = plot->measureAxises.at(type);
		const auto dimensionAxis = plot->dimensionAxises.at(type);

		const auto measureEnabled =
		    measureAxis.enabled.calculate<double>();

		drawTitle(plot->commonAxises.at(type).title,
		    dimensionAxis.enabled ? 1.0 : measureEnabled);

		if (dimensionAxis.enabled) drawDimension(dimensionAxis);

		if (measureEnabled > 0) drawMeasure(measureAxis);

		canvas.restore();
	}
}

void DrawLegend::drawTitle(const ::Anim::String &title, double mul)
{
	auto rect = contentRect;
	rect.size.y += titleHeight;
	title.visit(
	    [this, &rect, &mul](int, const auto &title)
	    {
		    DrawLabel(*this,
		        Geom::TransformedRect::fromRect(rect),
		        title.value,
		        style.title,
		        events.title,
		        Events::Targets::legendTitle(title.value, type),
		        DrawLabel::Options(true,
		            title.weight * weight * mul));
	    });
}

void DrawLegend::drawDimension(const Gen::DimensionAxis &axis)
{
	enabled = static_cast<double>(axis.enabled);

	for (const auto &value : axis) {
		if (value.second.weight > 0) {
			auto itemRect = getItemRect(value.second.range.getMin());
			if (itemRect.y().getMax() < contentRect.y().getMax()) {
				auto alpha = value.second.weight * weight * enabled;
				auto markerColor =
				    colorBuilder.render(value.second.colorBase)
				    * alpha;
				drawMarker(markerColor, getMarkerRect(itemRect));

				DrawLabel(*this,
				    getLabelRect(itemRect),
				    value.second.label,
				    style.label,
				    events.label,
				    Events::Targets::legendLabel(value.second.label,
				        type),
				    DrawLabel::Options(true, alpha));
			}
		}
	}
}

Geom::Rect DrawLegend::getItemRect(double index) const
{
	Geom::Rect res = contentRect;
	res.pos.y += titleHeight + index * itemHeight;
	res.size.y = itemHeight;
	if (res.size.x < 0) res.size.x = 0;
	return res;
}

Geom::Rect DrawLegend::getMarkerRect(const Geom::Rect &itemRect) const
{
	auto markerSize = style.marker.size->get(contentRect.size.y,
	    style.label.calculatedSize());
	Geom::Rect res = itemRect;
	res.pos.y += itemHeight / 2.0 - markerSize / 2.0;
	res.size = Geom::Size::Square(markerSize);
	return res;
}

Geom::TransformedRect DrawLegend::getLabelRect(
    const Geom::Rect &itemRect) const
{
	auto markerSize = style.marker.size->get(contentRect.size.y,
	    style.label.calculatedSize());
	Geom::Rect res = itemRect;
	res.pos.x += markerSize;
	res.size.x -= std::max(0.0, res.size.x - markerSize);
	return Geom::TransformedRect::fromRect(res);
}

void DrawLegend::drawMarker(const Gfx::Color &color,
    const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushColor(color);
	canvas.setLineColor(color);
	canvas.setLineWidth(0);

	auto radius = rootStyle.legend.marker.type->factor<double>(
	                  Styles::Legend::Marker::Type::circle)
	            * rect.size.minSize() / 2.0;

	auto markerElement = Events::Targets::legendMarker(type);

	if (events.marker->invoke(
	        Events::OnRectDrawEvent(*markerElement, {rect, false}))) {
		Gfx::Draw::RoundedRect(canvas, rect, radius);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(markerElement));
	}

	canvas.restore();
}

void DrawLegend::drawMeasure(const Gen::MeasureAxis &axis)
{
	enabled = axis.enabled.calculate<double>();

	axis.unit.visit(
	    [this, &axis](int, const auto &unit)
	    {
		    extremaLabel(axis.range.getMax(),
		        unit.value,
		        0,
		        unit.weight);
		    extremaLabel(axis.range.getMin(),
		        unit.value,
		        5,
		        unit.weight);
	    });

	auto bar = getBarRect();

	using ST = Gen::ChannelId;
	switch (type) {
	case ST::color: colorBar(bar); break;
	case ST::lightness: lightnessBar(bar); break;
	case ST::size: sizeBar(bar); break;
	default: break;
	}
}

void DrawLegend::extremaLabel(double value,
    const std::string &unit,
    int pos,
    double plusWeight)
{
	auto text = Text::SmartString::fromNumber(value,
	    *style.label.numberFormat,
	    static_cast<size_t>(*style.label.maxFractionDigits),
	    *style.label.numberScale,
	    unit);
	auto itemRect = getItemRect(pos);

	DrawLabel(*this,
	    getLabelRect(itemRect),
	    text,
	    style.label,
	    events.label,
	    Events::Targets::legendLabel(text, type),
	    DrawLabel::Options(true, weight * enabled * plusWeight));
}

void DrawLegend::colorBar(const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{*rootStyle.plot.marker.colorGradient
	                       * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(type);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		canvas.rectangle(rect);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	canvas.restore();
}

void DrawLegend::lightnessBar(const Geom::Rect &rect)
{
	Gfx::ColorGradient gradient;

	gradient.stops.emplace_back(0.0,
	    colorBuilder.render(Gen::ColorBase(0U, 0.0)));
	gradient.stops.emplace_back(0.5,
	    colorBuilder.render(Gen::ColorBase(0U, 0.5)));
	gradient.stops.emplace_back(1.0,
	    colorBuilder.render(Gen::ColorBase(0U, 1.0)));

	canvas.save();

	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{gradient * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(type);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		canvas.rectangle(rect);
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	canvas.restore();
}

void DrawLegend::sizeBar(const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushColor(Gfx::Color::Gray(0.8) * (weight * enabled));
	canvas.setLineWidth(0);

	auto barElement = Events::Targets::legendBar(type);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, {rect, false}))) {
		canvas.beginPolygon();
		canvas.addPoint(rect.bottomLeft());
		canvas.addPoint(rect.bottomRight());
		canvas.addPoint(rect.topSide().center());
		canvas.endPolygon();
		renderedChart.emplace(Geom::TransformedRect::fromRect(rect),
		    std::move(barElement));
	}

	canvas.restore();
}

Geom::Rect DrawLegend::getBarRect() const
{
	auto markerSize = style.marker.size->get(contentRect.size.y,
	    style.label.calculatedSize());
	Geom::Rect res = contentRect;
	res.pos.y += titleHeight + itemHeight / 2.0;
	res.size.y = 5 * itemHeight;
	res.size.x = markerSize;
	return res;
}

}