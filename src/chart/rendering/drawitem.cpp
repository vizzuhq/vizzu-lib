#include "drawitem.h"

#include "base/text/smartstring.h"
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

drawItem::drawItem(const Diag::DiagramItem &item,
    const DrawingContext &context) :
    DrawingContext(context), item(item)
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
	if ((double)item.enabled == 0) return;

	BlendedDrawItem blended(item,
	    options,
	    diagram.getStyle(),
	    diagram.getItems(),
	    0);

	auto baseColor = *style.color * (double)diagram.anyAxisSet;

	if ((double)blended.enabled > 0)
	{
		if ((double)guides.y.guidelines > 0)
		{
			auto lineColor = baseColor * (double)guides.y.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.xComp() + origo.yComp();
			painter.drawLine(Geom::Line(axisPoint, blended.center));
		}
		if ((double)guides.x.guidelines > 0)
		{
			blended.center.x = Math::interpolate(blended.center.x,
			    1.0,
			    (double)options.polar.get());
			auto lineColor = baseColor * (double)guides.x.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.yComp() + origo.xComp();
			painter.drawLine(Geom::Line(blended.center, axisPoint));
		}
	}
}

void drawItem::draw()
{
	if (!shouldDraw()) return;

	if (drawOptions.onlyEssentials()
	    && (double)diagram.anySelected
	    && (double)item.selected == 0)
		return;

	auto lineFactor =
	    (double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Line);

	auto circleFactor =
	    (double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Circle);

	if (lineFactor > 0 && circleFactor)
	{
		CircleItem circle(item,
		    options,
		    diagram.getStyle());

		LineItem line(item,
		    options,
		    diagram.getStyle(),
		    diagram.getItems(),
		    0);

		draw(circle, 1, false);
		draw(line, 1, true);
	}
	else
	{
		BlendedDrawItem blended0(item,
		    options,
		    diagram.getStyle(),
		    diagram.getItems(),
		    0);
/*
		BlendedDrawItem blended1(item,
		    options,
		    diagram.getStyle(),
		    diagram.getItems(),
		    1);
*/
		draw(blended0, (1-lineFactor) * (1-lineFactor), false);
		draw(blended0, sqrt(lineFactor), true);
//		draw(blended1, sqrt(lineFactor), true, false);
	}
}

bool drawItem::shouldDraw()
{
	bool enabled = (double)item.enabled > 0;
	if ((double)options.shapeType.get().getFactor(
	        Diag::ShapeType::Area)
	    > 0)
	{
		const auto *prev0 = ConnectingDrawItem::getPrev(item,
		    diagram.getItems(), 0);

		const auto *prev1 = ConnectingDrawItem::getPrev(item,
		    diagram.getItems(), 1);

		if (prev0) enabled |= (double)prev0->enabled > 0;
		if (prev1) enabled |= (double)prev1->enabled > 0;
	}
	return enabled;
}

void drawItem::draw(
	const DrawItem &drawItem,
    double factor,
    bool line,
	bool hasLabel)
{
	if ((double)drawItem.enabled > 0 && factor > 0)
	{
		painter.setPolygonMinDotSize(*style.data.circleMinRadius);
		painter.setPolygonToCircleFactor(line ? 0.0 : (double)drawItem.morphToCircle);
		painter.setPolygonStraightFactor((double)drawItem.linear);
		painter.setResMode(drawOptions.getResoultionMode());

		auto colors = getColor(drawItem, factor);

		if (line) {
			painter.drawStraightLine(
				drawItem.getLine(), drawItem.lineWidth,
				colors.second, colors.second * drawItem.connected);
		} else {
			canvas.setLineColor(colors.first);
			canvas.setLineWidth(
			    *style.plot.marker.borderWidth);
			canvas.setBrushColor(colors.second);
			painter.drawPolygon(drawItem.points);
			canvas.setLineWidth(0);
		}

		if (!drawOptions.onlyEssentials() && hasLabel)
			drawLabel(drawItem, colors.second);
	}
}

void drawItem::drawLabel(
    const DrawItem &drawItem,
    const Gfx::Color &color)
{
	const auto &val0 = item.label.values[0];
	const auto &val1 = item.label.values[1];

	auto weight = val0.weight;
	if (item.label.count == 2) weight += val1.weight;

	if (weight == 0.0) return;

	auto text = getLabelText();
	if (text.empty()) return;

	auto &labelStyle = style.plot.marker.label;
	auto padding = ((GUI::Margin)labelStyle).getSpace();
	canvas.setFont(Gfx::Font(labelStyle));
	auto neededSize = canvas.textBoundary(text);

	auto relVerPos = labelStyle.position->combine<Geom::Point>(
	[&](const auto &position)
	{
		switch (position)
		{
			case Styles::MarkerLabel::Position::center:
				return drawItem.getBoundary().center();
			case Styles::MarkerLabel::Position::below:
				return drawItem.getBoundary().leftSide().center();
			default:
			case Styles::MarkerLabel::Position::above:
				return drawItem.getBoundary().rightSide().center();
			};
	});

	auto relHorPos = labelStyle.position->combine<Geom::Point>(
	[&](const auto &position)
	{
		switch (position.value)
		{
		case Styles::MarkerLabel::Position::center:
			return drawItem.getBoundary().center();
		case Styles::MarkerLabel::Position::below:
			return drawItem.getBoundary().bottomSide().center();
		default:
		case Styles::MarkerLabel::Position::above:
			return drawItem.getBoundary().topSide().center();
		};
	});

	auto verPos = coordSys.convert(relVerPos);
	auto horPos = coordSys.convert(relHorPos);

	verPos = verPos + (Geom::Point() - neededSize.yComp() / 2);
	horPos = horPos + (Geom::Point() - neededSize.xComp() / 2);

	verPos = labelStyle.position->combine<Geom::Point>(
	[&](const auto &position)
	{
		switch (position.value)
		{
		case Styles::MarkerLabel::Position::center:
			return verPos - neededSize.xComp() / 2;
		case Styles::MarkerLabel::Position::below:
			return verPos - neededSize.xComp() - padding.xComp();
		default:
		case Styles::MarkerLabel::Position::above:
			return verPos + padding.xComp();
		};
	});

	horPos = labelStyle.position->combine<Geom::Point>(
	[&](const auto &position)
	{
		switch (position.value)
		{
		case Styles::MarkerLabel::Position::center:
			return horPos - neededSize.yComp() / 2;
		case Styles::MarkerLabel::Position::below:
			return horPos + padding.yComp();
		default:
		case Styles::MarkerLabel::Position::above:
			return horPos - neededSize.yComp() - padding.yComp();
		};
	});

	auto pos = Math::interpolate(verPos, horPos,
	    (double)options.horizontal.get());

	auto rect = Geom::Rect(pos, neededSize);
	auto textColor = (*labelStyle.filter)(color) * weight;
	auto textBgColor = *labelStyle.backgroundColor * weight;
	if (!textBgColor.isTransparent())
	{
		canvas.setBrushColor(textBgColor);
		canvas.setLineColor(textBgColor);
		canvas.rectangle(rect);
	}
	canvas.setTextColor(textColor);
	canvas.text(rect, text);
}

std::string drawItem::getLabelText()
{
	auto &labelStyle = style.plot.marker.label;

	const auto &val0 = item.label.values[0];
	const auto &val1 = item.label.values[1];

	auto value = item.label.count == 1 ? val0.value.value :
				 val0.value.value * val0.weight
				+ val1.value.value * val1.weight;

	auto text = Text::SmartString::fromNumber(value,
	    *labelStyle.numberFormat);

	if (*labelStyle.numberFormat != Text::NumberFormat::prefixed)
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
    double factor)
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

	auto alpha = (double)drawItem.enabled * factor;

	auto finalBorderColor = actBorderColor * alpha;
	auto itemColor = selectedColor * alpha * fillAlpha;

	return std::make_pair(finalBorderColor, itemColor);
}

Gfx::Color drawItem::getSelectedColor()
{
	auto orig = item.color;

	auto gray = orig.desaturate().lightnessScaled(0.75);
	auto interpolated =
	    Math::interpolate(gray, orig, (double)item.selected);

	return Math::interpolate(
		item.color,
	    interpolated,
	    (double)diagram.anySelected);
}
