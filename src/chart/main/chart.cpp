#include "chart.h"

#include "chart/options/advancedoptions.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawdiagram.h"
#include "chart/rendering/drawitem.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/drawlegend.h"
#include "chart/rendering/logo.h"
#include "data/datacube/datacube.h"

using namespace Vizzu;

Chart::Chart() :
    animator(std::make_shared<Anim::Animator>()),
    stylesheet(Styles::Chart::def()),
	events(*this)
{
	stylesheet.setActiveParams(actStyles);
	getEvents().createEvents();
	nextOptions = std::make_shared<Diag::Options>();

	animator->onDraw = [&](Diag::DiagramPtr actDiagram)
	{
		events.invoke(events.update);
		this->actDiagram = std::move(actDiagram);
		if (onChanged) onChanged();
	};

	animator->init(diagram(nextOptions));
}

void Chart::setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info)
{
	if (actDiagram) {
		layout.setBoundary(rect, *actDiagram, info);
	}
}

void Chart::animate(std::function<void()> onComplete)
{
	animator->animate(diagram(nextOptions), onComplete);
	nextOptions = std::make_shared<Diag::Options>(*nextOptions);
}

Diag::Descriptor Chart::getDescriptor()
{
    return Diag::Descriptor(getSetter());
}

Diag::OptionsSetterPtr Chart::getSetter()
{
	auto setter = std::make_shared<Diag::AdvancedOptions>(*nextOptions);
	setter->setTable(&table);
	return setter;
}

void Chart::draw(Gfx::ICanvas &canvas) const
{
	if (actDiagram)
	{
		Draw::drawBackground(layout.boundary,
		    canvas,
		    actDiagram->getStyle());

		actDiagram->getOptions()->title.get().visit(
		[&](const auto &title)
		{
			if (title.value.has_value())
				Draw::drawLabel(layout.title,
					*title.value,
					actDiagram->getStyle().title,
					canvas, true, title.weight);
		});

		Draw::drawDiagram(layout.plot,
		    *actDiagram,
		    canvas,
		    Draw::DrawOptions(false, false),
		    actDiagram->getStyle());

		actDiagram->getOptions()->legend.get().visit(
		[&](const auto &legend)
		{
			if (legend.value)
				Draw::drawLegend(layout.legend,
					*actDiagram,
					canvas,
					*legend.value,
					legend.weight);
		});
	}

	Draw::Logo(canvas).draw(
		layout.boundary.topRight() - Geom::Point(55, 15),
		40, false,
		Gfx::Color::Gray(0.85));
}

Diag::DiagramPtr Chart::diagram(
    Diag::DiagramOptionsPtr options) const
{
	return std::make_shared<Diag::Diagram>(table,
	    options,
	    stylesheet.getFullParams());
}

const Diag::Marker *Chart::markerAt(const Geom::Point &point) const
{
	if (animator->isRunning()) return nullptr;

	if (actDiagram) for (const auto &marker : actDiagram->getMarkers())
	{
		const auto &plotArea = layout.plotArea;
		const auto &options = *actDiagram->getOptions();

		Draw::CoordinateSystem coordSys(plotArea,
			options.angle.get(),
			options.polar.get(),
			actDiagram->keepAspectRatio);

		auto drawItem = Draw::DrawItem::create(marker,
			options,
			actDiagram->getStyle(),
			actDiagram->getMarkers());

		if (drawItem->bounds(coordSys.getOriginal(point)))
			return &marker;
	}
	return nullptr;
}
