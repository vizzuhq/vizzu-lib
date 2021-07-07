#include "drawguides.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawGuides::drawGuides(const DrawingContext &context,
    const Guides &guides) :
    DrawingContext(context),
    guides(guides)
{
	draw(true);
	draw(false);
}

void drawGuides::draw(bool horizontal)
{
	const auto &guideStyle = style.plot.axis.guides;

	auto baseColor = *guideStyle.color;
	if (baseColor.alpha == 0) return;

	const auto &axises = diagram.discreteAxises;
	auto axisId = horizontal ? Diag::Scale::Type::X : Diag::Scale::Type::Y;
	const auto &axis = axises.at(axisId);

	if (axis.enabled
		&& *guideStyle.lineWidth > 0
		&& ((double)guides.at(axisId).discreteGuides > 0))
	{
		canvas.setLineWidth(*guideStyle.lineWidth);

		Diag::DiscreteAxis::Values::const_iterator it;
		for (it = axis.begin();
			 it != axis.end();
			 ++it)
		{
			auto weight = it->second.weight;
			weight *= (double)guides.at(axisId).discreteGuides;
			if (weight == 0) continue;

			auto next = it;
			next++;
			if (next != axis.end())
			{
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
	auto ident = Geom::Point::Ident(horizontal);
	auto normal = Geom::Point::Ident(!horizontal);
	auto relMax = ident * val;

	canvas.setLineColor(color);
	Geom::Line line(relMax, relMax + normal);
	if (events.plot.axis.guide->invoke(Events::OnLineDrawParam(line)))
		painter.drawLine(line);
}

