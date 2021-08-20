#include "drawitem.h"

#include "base/geom/angle.h"
#include "base/text/smartstring.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/draworientedlabel.h"
#include "chart/rendering/items/areaitem.h"
#include "chart/rendering/items/blendeditem.h"
#include "chart/rendering/items/circleitem.h"
#include "chart/rendering/items/drawitem.h"
#include "chart/rendering/items/lineitem.h"
#include "chart/rendering/items/rectangleitem.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawItem::drawItem(const Diag::Marker &marker,
    const DrawingContext &context) :
    DrawingContext(context), marker(marker)
{}

bool drawItem::mayDrawLines(const Guides &guides)
{
	return (double)guides.y.guidelines > 0
	        || (double)guides.x.guidelines > 0;
}

void drawItem::drawLines(const Guides &guides,
    const Styles::Guide &style,
    const Geom::Point &origo)
{
	if ((double)marker.enabled == 0) return;

	BlendedDrawItem blended(marker,
	    options,
	    diagram.getStyle(),
		coordSys,
	    diagram.getMarkers(),
	    0);

	auto baseColor = *style.color * (double)diagram.anyAxisSet;

	if ((double)blended.enabled > 0)
	{
		if ((double)guides.y.guidelines > 0)
		{
			auto lineColor = baseColor * (double)guides.y.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.xComp() + origo.yComp();
			Geom::Line line(axisPoint, blended.center);
			if (events.plot.marker.guide
				->invoke(Events::OnLineDrawParam(line)))
			{
				painter.drawLine(line);
			}
		}
		if ((double)guides.x.guidelines > 0)
		{
			blended.center.x = Math::interpolate(blended.center.x,
			    1.0,
			    (double)options.polar.get());
			auto lineColor = baseColor * (double)guides.x.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.yComp() + origo.xComp();
			Geom::Line line(blended.center, axisPoint);
			if (events.plot.marker.guide
				->invoke(Events::OnLineDrawParam(line)))
			{
				painter.drawLine(line);
			}
		}
	}
}

void drawItem::draw()
{
	if (!shouldDraw()) return;

	if (drawOptions.onlyEssentials()
	    && (double)diagram.anySelected
	    && (double)marker.selected == 0)
		return;

	auto lineFactor =
	    (double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Line);

	auto circleFactor =
	    (double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Circle);

	if (lineFactor > 0 && circleFactor)
	{
		CircleItem circle(marker,
		    options,
		    diagram.getStyle(),
			coordSys);

		LineItem line(marker,
		    options,
		    diagram.getStyle(),
			coordSys,
		    diagram.getMarkers(),
		    0);

		draw(circle, 1, false);
		draw(line, 1, true);
	}
	else
	{
		BlendedDrawItem blended0(marker,
		    options,
		    diagram.getStyle(),
			coordSys,
		    diagram.getMarkers(),
		    0);
/*
		BlendedDrawItem blended1(marker,
		    options,
		    diagram.getStyle(),
		    diagram.getMarkers(),
		    1);
*/
		draw(blended0, (1-lineFactor) * (1-lineFactor), false);
		draw(blended0, sqrt(lineFactor), true);
//		draw(blended1, sqrt(lineFactor), true, false);
	}
}

void drawItem::drawLabel()
{
	if ((double)marker.enabled == 0) return;

	BlendedDrawItem blended0(marker,
	    options,
	    diagram.getStyle(),
		coordSys,
	    diagram.getMarkers(),
	    0);
	
	drawLabel(blended0);
}

bool drawItem::shouldDraw()
{
	bool enabled = (double)marker.enabled > 0;
	if ((double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Area)
	    > 0)
	{
		const auto *prev0 = ConnectingDrawItem::getPrev(marker,
		    diagram.getMarkers(), 0);

		const auto *prev1 = ConnectingDrawItem::getPrev(marker,
		    diagram.getMarkers(), 1);

		if (prev0) enabled |= (double)prev0->enabled > 0;
		if (prev1) enabled |= (double)prev1->enabled > 0;
	}
	return enabled;
}

void drawItem::draw(
	const DrawItem &drawItem,
    double factor,
    bool line)
{
	if ((double)drawItem.enabled == 0 || factor == 0) return;

	painter.setPolygonMinDotSize(*style.data.circleMinRadius);
	painter.setPolygonToCircleFactor(line ? 0.0 : (double)drawItem.morphToCircle);
	painter.setPolygonStraightFactor((double)drawItem.linear);
	painter.setResMode(drawOptions.getResoultionMode());

	auto colors = getColor(drawItem, factor);

	if (line) 
	{
		if (events.plot.marker.base
			->invoke(Events::OnRectDrawParam(drawItem.getBoundary())))
		{
			painter.drawStraightLine(
				drawItem.getLine(), drawItem.lineWidth,
				colors.second, colors.second * drawItem.connected);
		}
	}
	else 
	{
		canvas.setLineColor(colors.first);
		canvas.setLineWidth(
			*style.plot.marker.borderWidth);
		canvas.setBrushColor(colors.second);
		if (events.plot.marker.base
			->invoke(Events::OnRectDrawParam(drawItem.getBoundary())))
		{
			painter.drawPolygon(drawItem.points);
		}
		canvas.setLineWidth(0);
	}
}

void drawItem::drawLabel(const DrawItem &drawItem)
{
	if ((double)drawItem.labelEnabled == 0) return;

	auto color = getColor(drawItem, 1, true).second;

	auto weight = marker.label.factor();
	if (weight == 0.0) return;

	auto text = getLabelText();
	if (text.empty()) return;

	auto &labelStyle = style.plot.marker.label;

	auto labelPos = labelStyle.position->combine<Geom::Line>(
		[&](const auto &position){ 
			return drawItem.getLabelPos(position, coordSys); 
		});

	auto textColor = (*labelStyle.filter)(color) * weight;
	auto bgColor = *labelStyle.backgroundColor * weight;

	auto centered = 
		labelStyle.position->factor(Styles::MarkerLabel::Position::center);

	drawOrientedLabel(*this, text, labelPos, labelStyle,
		centered, textColor, bgColor);
}

std::string drawItem::getLabelText()
{
	auto &labelStyle = style.plot.marker.label;

	const auto &val0 = marker.label.values[0];
	const auto &val1 = marker.label.values[1];

	auto value = marker.label.count == 1 ? val0.value.value :
				 val0.value.value * val0.weight
				+ val1.value.value * val1.weight;

	auto text = Text::SmartString::fromNumber(value,
	    *labelStyle.numberFormat);

	if (val0.value.hasValue 
		&& !val0.value.unit.empty()
		&& *labelStyle.numberFormat != Text::NumberFormat::prefixed)
		text += " ";

	auto text0 = val0.value.hasValue ? text + val0.value.unit : std::string();
	auto idx0 = val0.value.indexStr;

	// todo: interpolate Format
	typedef Styles::MarkerLabel::Format Format;
	switch((Format)*labelStyle.format)
	{
	default:
	case Format::valueFirst:
		if (!idx0.empty())
		{
			if (!text0.empty()) text0 += ", ";
			text0 += idx0;
		}
		text = text0;
		break;

	case Format::categoriesFirst:
		if (!text0.empty())
		{
			if (!idx0.empty()) idx0 += ", ";
			idx0 += text0;
		}
		text = idx0;
		break;
	}
	return text;
}

std::pair<Gfx::Color, Gfx::Color> drawItem::getColor(
    const DrawItem &drawItem,
    double factor,
	bool label)
{
	auto selectedColor = getSelectedColor();

	auto borderAlpha = *style.plot.marker.borderOpacity;
	auto fillAlpha = *style.plot.marker.fillOpacity;

	auto fakeBgColor =
	    (*style.backgroundColor + *style.plot.backgroundColor)
	        .transparent(1.0);

	auto borderColor = style.plot.marker.borderOpacityMode
		->combine<Gfx::Color>([&](const auto &mode)
	{
		if (mode == Styles::Marker::BorderOpacityMode::premultiplied)
			return Math::interpolate(fakeBgColor, selectedColor, borderAlpha);
		else
			return selectedColor * borderAlpha;
	});

	auto actBorderColor = Math::interpolate(selectedColor,
		borderColor,
		(double)drawItem.border);

	const auto &enabled = label ? drawItem.labelEnabled : drawItem.enabled;
	auto alpha = (double)enabled * factor;

	auto finalBorderColor = actBorderColor * alpha;
	auto itemColor = selectedColor * alpha * fillAlpha;

	return std::make_pair(finalBorderColor, itemColor);
}

Gfx::Color drawItem::getSelectedColor()
{
	auto orig = marker.color;

	auto gray = orig.desaturate().lightnessScaled(0.75);
	auto interpolated =
	    Math::interpolate(gray, orig, (double)marker.selected);

	return Math::interpolate(
		marker.color,
	    interpolated,
	    (double)diagram.anySelected);
}
