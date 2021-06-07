#include "drawdiagram.h"

#include "base/text/smartstring.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawaxes.h"

#include "drawitem.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawDiagram::drawDiagram(const Geom::Rect &rect,
    const Diag::Diagram &diagram,
    Gfx::ICanvas &canvas,
    const DrawOptions &drawOptions,
    const Styles::Chart &style) :
	DrawingContext(rect, diagram, canvas, drawOptions, style)
{
	guides.init(diagram.axises, options);

	if (!drawOptions.onlyEssentials())
	{
		drawBackground(rect, canvas, style.plot);

		drawAxes(*this, guides).drawBase();

		drawItemGuides();
	}

	drawItems();

	if (!drawOptions.onlyEssentials())
	{
		drawAxes(*this, guides).drawLabels();
	}
}

void drawDiagram::drawItemGuides()
{
	auto &style = diagram.getStyle().plot.marker.guides;

	if (!style.color->isTransparent()
		&& *style.lineWidth > 0
		&& (double)diagram.anyAxisSet > 0
		&& drawItem::mayDrawLines(guides))
	{
		canvas.setLineWidth(*style.lineWidth);

		auto origo = diagram.axises.origo();

		for (const auto &item : diagram.getItems())
			drawItem(item, *this).drawLines(guides, style, origo);

		canvas.setLineWidth(0);
	}
}

void drawDiagram::drawItems()
{
	if (*style.plot.marker.renderingOrder
	    == Styles::Marker::RenderingOrder::reverse)
	{
		for (const auto &item : Util::Reverse(diagram.getItems()))
			drawItem(item, *this).draw();
	}
	else
	{
		for (const auto &item : diagram.getItems())
			drawItem(item, *this).draw();
	}
}
