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

void DrawGuides::draw()
{
	draw(Gen::AxisId::x);
	draw(Gen::AxisId::y);
}

void DrawGuides::draw(Gen::AxisId axisId)
{
	const auto &guideStyle = rootStyle.plot.getAxis(axisId).guides;

	auto baseColor = *guideStyle.color;

	if (const auto &axis = plot->axises.at(axisId).dimension;
	    !baseColor.isTransparent() && !axis.empty()
	    && *guideStyle.lineWidth > 0
	    && plot->guides.at(axisId).axisGuides != false) {
		canvas.setLineWidth(*guideStyle.lineWidth);

		for (auto it = axis.begin(), end = std::prev(axis.end());
		     it != end;
		     ++it) {
			if (auto &&weight = (*it).weight(axis.factor); weight > 0)
				drawGuide(axisId,
				    (*it).range.getMax(),
				    baseColor
				        * Math::FuzzyBool::And<double>(weight,
				            plot->guides.at(axisId).axisGuides));
		}

		canvas.setLineWidth(0);
	}
}

void DrawGuides::drawGuide(Gen::AxisId axisId,
    double val,
    const Gfx::Color &color)
{
	auto eventTarget = Events::Targets::axisGuide(axisId);

	auto ident = Geom::Point::Ident(+axisId);
	auto normal = Geom::Point::Ident(!+axisId);
	auto relMax = ident * val;

	canvas.setLineColor(color);
	const Geom::Line line(relMax, relMax + normal);
	if (rootEvents.draw.plot.axis.guide->invoke(
	        Events::OnLineDrawEvent(*eventTarget, {line, true}))) {
		painter.drawLine(line);
		renderedChart.emplace(Draw::Line{line, true},
		    std::move(eventTarget));
	}
}

}