#include "drawguides.h"

#include <iterator>
#include <utility>

#include "base/geom/line.h"
#include "base/geom/point.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"

#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawGuides::draw()
{
	draw(true);
	draw(false);
}

void DrawGuides::draw(bool horizontal)
{
	auto axisId = horizontal ? Gen::ChannelId::x : Gen::ChannelId::y;

	const auto &guideStyle = rootStyle.plot.getAxis(axisId).guides;

	auto baseColor = *guideStyle.color;
	if (baseColor.alpha == 0) return;

	const auto &axises = plot->axises;
	const auto &axis = axises.at(axisId).dimension;

	if (axis.enabled && *guideStyle.lineWidth > 0
	    && (static_cast<double>(plot->guides.at(axisId).axisGuides)
	        > 0)) {
		canvas.setLineWidth(*guideStyle.lineWidth);

		for (auto it = axis.begin(); it != axis.end(); ++it) {
			auto weight = it->second.weight;
			weight *= static_cast<double>(
			    plot->guides.at(axisId).axisGuides);
			if (weight == 0) continue;

			auto next = std::next(it);
			if (next != axis.end()) {
				drawGuide(horizontal,
				    it->second.range.getMax(),
				    baseColor * weight);
			}
		}

		canvas.setLineWidth(0);
	}
}

void DrawGuides::drawGuide(bool horizontal,
    double val,
    const Gfx::Color &color)
{
	auto eventTarget = Events::Targets::axisGuide(horizontal);

	auto ident = Geom::Point::Ident(horizontal);
	auto normal = Geom::Point::Ident(!horizontal);
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