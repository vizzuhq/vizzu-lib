#include "drawguides.h"

#include <iterator>
#include <utility>

#include "base/geom/line.h"
#include "base/geom/point.h"
#include "base/math/fuzzybool.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"

#include "renderedchart.h"

namespace Vizzu::Draw
{
void DrawGuides::draw(Gen::AxisId axisId)
{
	const auto &guideStyle =
	    parent.rootStyle.plot.getAxis(axisId).guides;

	auto baseColor = *guideStyle.color;

	if (!baseColor.isTransparent() && *guideStyle.lineWidth > 0
	    && parent.plot->guides.at(axisId).axisGuides != false) {
		parent.canvas.setLineWidth(*guideStyle.lineWidth);

		for (const auto &sep : parent.getSeparators(axisId))
			drawGuide(axisId,
			    sep.position,
			    baseColor
			        * Math::FuzzyBool::And<double>(sep.weight,
			            parent.plot->guides.at(axisId).axisGuides));

		parent.canvas.setLineWidth(0);
	}
}

void DrawGuides::drawGuide(Gen::AxisId axisId,
    double val,
    const Gfx::Color &color)
{
	auto eventTarget = Events::Targets::axisGuide(axisId);

	auto ident = Geom::Point::Ident(++axisId);
	auto normal = Geom::Point::Ident(++!axisId);
	auto relMax = ident * val;

	parent.canvas.setLineColor(color);
	const Geom::Line line(relMax, relMax + normal);
	if (parent.rootEvents.draw.plot.axis.guide->invoke(
	        Events::OnLineDrawEvent(*eventTarget, {line, true}))) {
		parent.painter.drawLine(line);
		parent.renderedChart.emplace(Line{line, true},
		    std::move(eventTarget));
	}
}

}