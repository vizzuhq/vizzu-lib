#include "chart.h"

#include "chart/options/advancedoptions.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawdiagram.h"
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

void Chart::animate(std::function<void()> onFinished)
{
	animator->animate(diagram(nextOptions), onFinished);
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

::Anim::Control &Chart::getAnimControl()
{
	return *animator;
}

Events& Chart::getEvents() {
	return events;
}

Util::EventDispatcher &Chart::getEventDispatcher() {
	return eventDispatcher;
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
