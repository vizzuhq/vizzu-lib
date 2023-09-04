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
    events(context.rootEvents.legend),
    style(context.rootStyle.legend),
    type(channelType),
    weight(weight),
    itemHeight(DrawLabel::getHeight(style.label, canvas)),
    titleHeight(DrawLabel::getHeight(style.title, canvas))
{
	contentRect = style.contentRect(layout.legend,
	    context.rootStyle.calculatedSize());

	DrawBackground(layout.legend,
	    canvas,
	    style,
	    events.background,
	    Events::OnRectDrawParam("legend"));

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
		    Events::Events::OnTextDrawParam param("legend.title");
		    DrawLabel(rect,
		        title.value,
		        style.title,
		        events.title,
		        std::move(param),
		        canvas,
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
				Events::Events::OnTextDrawParam param("legend.label");
				DrawLabel(getLabelRect(itemRect),
				    value.second.label,
				    style.label,
				    events.label,
				    std::move(param),
				    canvas,
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

Geom::Rect DrawLegend::getLabelRect(const Geom::Rect &itemRect) const
{
	auto markerSize = style.marker.size->get(contentRect.size.y,
	    style.label.calculatedSize());
	Geom::Rect res = itemRect;
	res.pos.x += markerSize;
	res.size.x -= std::max(0.0, res.size.x - markerSize);
	return res;
}

void DrawLegend::drawMarker(Gfx::Color color, const Geom::Rect &rect)
{
	canvas.setBrushColor(color);
	canvas.setLineColor(color);
	canvas.setLineWidth(0);

	auto radius = plot.getStyle().legend.marker.type->factor<double>(
	                  Styles::Legend::Marker::Type::circle)
	            * rect.size.minSize() / 2.0;

	if (events.marker->invoke(
	        Events::OnRectDrawParam("legend.marker", rect)))
		Gfx::Draw::RoundedRect(canvas, rect, radius);
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
	Events::Events::OnTextDrawParam param("legend.label");
	DrawLabel(getLabelRect(itemRect),
	    text,
	    style.label,
	    events.label,
	    std::move(param),
	    canvas,
	    DrawLabel::Options(true, weight * enabled));
}

void DrawLegend::colorBar(const Geom::Rect &rect)
{
	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{*plot.getStyle().plot.marker.colorGradient
	                       * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);
	if (events.bar->invoke(
	        Events::OnRectDrawParam("legend.bar", rect)))
		canvas.rectangle(rect);
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

	canvas.setBrushGradient(rect.leftSide(),
	    Gfx::ColorGradient{gradient * (weight * enabled)});
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);
	if (events.bar->invoke(
	        Events::OnRectDrawParam("legend.bar", rect)))
		canvas.rectangle(rect);
}

void DrawLegend::sizeBar(const Geom::Rect &rect)
{
	canvas.setBrushColor(Gfx::Color::Gray(0.8) * (weight * enabled));
	if (events.bar->invoke(
	        Events::OnRectDrawParam("legend.bar", rect))) {
		canvas.beginPolygon();
		canvas.addPoint(rect.bottomLeft());
		canvas.addPoint(rect.bottomRight());
		canvas.addPoint(rect.topSide().center());
		canvas.endPolygon();
	}
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
