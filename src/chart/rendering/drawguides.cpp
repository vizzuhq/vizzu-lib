#include "drawguides.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawGuides::drawGuides(const DrawingContext &context) :
    DrawingContext(context)
{
	draw(true);
	draw(false);
}

void drawGuides::draw(bool horizontal)
{
	auto axisId = horizontal ? Diag::ChannelId::x : Diag::ChannelId::y;

	const auto &guideStyle = style.plot.getAxis(axisId).guides;

	auto baseColor = *guideStyle.color;
	if (baseColor.alpha == 0) return;

	const auto &axises = diagram.discreteAxises;
	const auto &axis = axises.at(axisId);

	if (axis.enabled && *guideStyle.lineWidth > 0
	    && ((double)diagram.guides.at(axisId).discreteGuides > 0)) {
		canvas.setLineWidth(*guideStyle.lineWidth);

		Diag::DiscreteAxis::Values::const_iterator it;
		for (it = axis.begin(); it != axis.end(); ++it) {
			auto weight = it->second.weight;
			weight *=
			    (double)diagram.guides.at(axisId).discreteGuides;
			if (weight == 0) continue;

			auto next = it;
			next++;
			if (next != axis.end()) {
				drawGuide(horizontal,
				    it->second.range.getMax(),
				    baseColor * weight);
			}
		}

		canvas.setLineWidth(0);
	}
}

void drawGuides::drawGuide(bool horizontal,
    double val,
    const Gfx::Color &color)
{
	const char *element =
	    horizontal ? "plot.xAxis.guide" : "plot.yAxis.guide";
	auto ident = Geom::Point::Ident(horizontal);
	auto normal = Geom::Point::Ident(!horizontal);
	auto relMax = ident * val;

	canvas.setLineColor(color);
	Geom::Line line(relMax, relMax + normal);
	if (events.plot.axis.guide->invoke(
	        Events::OnLineDrawParam(element, line)))
		painter.drawLine(line);
}
