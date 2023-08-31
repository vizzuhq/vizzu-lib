#include "drawlegend.h"

#include "base/gfx/draw/roundedrect.h"
#include "chart/generator/colorbuilder.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

DrawLegend::DrawLegend(const DrawingContext &context,
    Gen::ChannelId channelType,
    double weight) :
    DrawingContext(context),
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
	    std::make_unique<Events::Targets::Legend>(channelType));

	if (static_cast<std::size_t>(type)
	    < std::size(plot.axises.axises)) {
		canvas.save();
		canvas.setClipRect(contentRect);

		const auto axis = plot.axises.at(type);
		const auto dimensionAxis = plot.dimensionAxises.at(type);

		if (static_cast<double>(dimensionAxis.enabled) > 0)
			drawDimension(dimensionAxis);

		if (axis.enabled.calculate<double>() > 0) drawMeasure(axis);

		canvas.restore();
	}
}

void DrawLegend::drawTitle(const ::Anim::String &title)
{
	auto rect = contentRect;
	rect.size.y += titleHeight;
	title.visit(
	    [this, &rect](int, const auto &title)
	    {
		    DrawLabel(*this,
		        Geom::TransformedRect::fromRect(rect),
		        title.value,
		        style.title,
		        events.title,
		        std::make_unique<Events::Targets::LegendTitle>
		            (title.value, type),
		        DrawLabel::Options(true,
		            title.weight * weight * enabled));
	    });
}

void DrawLegend::drawDimension(const Gen::DimensionAxis &axis)
{
	enabled = static_cast<double>(axis.enabled);

	drawTitle(axis.title);

	for (const auto &value : axis) {
		if (value.second.weight > 0) {
			auto itemRect = getItemRect(value.second.range.getMin());
			if (itemRect.y().getMax() < contentRect.y().getMax()) {
				auto alpha = value.second.weight * weight * enabled;
				auto markerColor = value.second.color * alpha;
				drawMarker(markerColor, getMarkerRect(itemRect));

				DrawLabel(*this,
				    getLabelRect(itemRect),
				    value.second.label,
				    style.label,
				    events.label,
				    std::make_unique<Events::Targets::LegendLabel>
				        (value.second.label, type),
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

Geom::TransformedRect DrawLegend::getLabelRect(const Geom::Rect &itemRect) const
{
	auto markerSize = style.marker.size->get(contentRect.size.y,
	    style.label.calculatedSize());
	Geom::Rect res = itemRect;
	res.pos.x += markerSize;
	res.size.x -= std::max(0.0, res.size.x - markerSize);
	return Geom::TransformedRect::fromRect(res);
}

void DrawLegend::drawMarker(
	Gfx::Color color, 
	const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushColor(color);
	canvas.setLineColor(color);
	canvas.setLineWidth(0);

	auto radius = plot.getStyle().legend.marker.type->factor<double>(
	                  Styles::Legend::Marker::Type::circle)
	            * rect.size.minSize() / 2.0;

	auto markerElement = std::make_unique<Events::Targets::LegendMarker>(type);

	if (events.marker->invoke(
	        Events::OnRectDrawEvent(*markerElement, { rect, false })))
	{
		Gfx::Draw::RoundedRect(canvas, rect, radius);
		renderedChart->emplace(Geom::TransformedRect::fromRect(rect), 
			std::move(markerElement));
	}

	canvas.restore();
}

void DrawLegend::drawMeasure(const Gen::Axis &axis)
{
	enabled = axis.enabled.calculate<double>();

	drawTitle(axis.title);

	extremaLabel(axis.range.getMax(), 0);
	extremaLabel(axis.range.getMin(), 5);

	auto bar = getBarRect();

	using ST = Gen::ChannelId;
	switch (type) {
	case ST::color: colorBar(bar); break;
	case ST::lightness: lightnessBar(bar); break;
	case ST::size: sizeBar(bar); break;
	default: break;
	}
}

void DrawLegend::extremaLabel(double value, int pos)
{
	auto text = Text::SmartString::fromNumber(value,
	    *style.label.numberFormat,
	    static_cast<size_t>(*style.label.maxFractionDigits),
	    *style.label.numberScale);
	auto itemRect = getItemRect(pos);

	auto labelElement = std::make_unique
		<Events::Targets::LegendLabel>(text, type);

	DrawLabel(*this,
	    getLabelRect(itemRect),
	    text,
	    style.label,
	    events.label,
	    std::move(labelElement),
	    DrawLabel::Options(true, weight * enabled));
}

void DrawLegend::colorBar(const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{*plot.getStyle().plot.marker.colorGradient
	        * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);

	auto barElement = std::make_unique<Events::Targets::LegendBar>(type);

	if (events.bar->invoke(Events::OnRectDrawEvent(*barElement, { rect, false })))
	{
		canvas.rectangle(rect);
		renderedChart->emplace(Geom::TransformedRect::fromRect(rect), 
			std::move(barElement));
	}

	canvas.restore();
}

void DrawLegend::lightnessBar(const Geom::Rect &rect)
{
	Gfx::ColorGradient gradient;
	const auto &style = plot.getStyle().plot.marker;

	auto range = style.lightnessRange();
	const auto &palette = *style.colorPalette;
	gradient.stops.emplace_back(0.0,
	    Gen::ColorBuilder(range, palette, 0, 0.0).render());
	gradient.stops.emplace_back(0.5,
	    Gen::ColorBuilder(range, palette, 0, 0.5).render());
	gradient.stops.emplace_back(1.0,
	    Gen::ColorBuilder(range, palette, 0, 1.0).render());

	canvas.save();

	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{gradient * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);

	auto barElement = std::make_unique<Events::Targets::LegendBar>(type);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, { rect, false })))
	{
		canvas.rectangle(rect);
		renderedChart->emplace(Geom::TransformedRect::fromRect(rect), 
			std::move(barElement));
	}

	canvas.restore();
}

void DrawLegend::sizeBar(const Geom::Rect &rect)
{
	canvas.save();

	canvas.setBrushColor(Gfx::Color::Gray(0.8) * (weight * enabled));
	canvas.setLineWidth(0);

	auto barElement = std::make_unique<Events::Targets::LegendBar>(type);

	if (events.bar->invoke(
	        Events::OnRectDrawEvent(*barElement, { rect, false }))) 
	{
		canvas.beginPolygon();
		canvas.addPoint(rect.bottomLeft());
		canvas.addPoint(rect.bottomRight());
		canvas.addPoint(rect.topSide().center());
		canvas.endPolygon();
		renderedChart->emplace(Geom::TransformedRect::fromRect(rect), 
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
