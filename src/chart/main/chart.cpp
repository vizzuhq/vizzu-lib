#include "chart.h"

#include "chart/options/advancedoptions.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawdiagram.h"
#include "chart/rendering/drawitem.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/drawmarkerinfo.h"
#include "chart/rendering/drawlegend.h"
#include "chart/rendering/logo.h"
#include "data/datacube/datacube.h"

using namespace Vizzu;

Chart::Chart() :
    animator(std::make_shared<Anim::Animator>()),
    stylesheet(Styles::Chart::def()),
	events(*this)
{
	computedStyles = stylesheet.getDefaultParams();
	stylesheet.setActiveParams(actStyles);
	nextOptions = std::make_shared<Diag::Options>();

	animator->onDraw.attach([&](Diag::DiagramPtr actDiagram)
	{
		this->actDiagram = std::move(actDiagram);
		if (onChanged) onChanged();
	});
	animator->onProgress.attach([&]() {
		events.update->invoke(Events::OnUpdateParam(animator->getControl()));
	});
	animator->onBegin = [&]() {
		events.animation.begin->invoke(Util::EventDispatcher::Params{this});
	};
	animator->onComplete = [&]() {
		events.animation.complete->invoke(Util::EventDispatcher::Params{this});
	};
}

void Chart::setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info)
{
	if (actDiagram) {
		actDiagram->getStyle().setup();
		layout.setBoundary(rect, *actDiagram, info);
	} else {
		layout.setBoundary(rect, info);
	}
}

void Chart::animate(OnComplete onComplete)
{
	auto f = [=](Diag::DiagramPtr diagram, bool ok) {
		actDiagram = diagram;
		if (ok) {
			prevOptions = *nextOptions;
			prevStyles = actStyles;
		}
		else {
			*nextOptions = prevOptions;
			actStyles = prevStyles;
			computedStyles = diagram->getStyle();
		}
		if (onComplete)
			onComplete(ok);
	};
	animator->animate(nextAnimOptions.control, f);
	nextAnimOptions = Anim::Options();
	nextOptions = std::make_shared<Diag::Options>(*nextOptions);
}

void Chart::setKeyframe()
{
	animator->addKeyframe(diagram(nextOptions), nextAnimOptions.keyframe);
	nextAnimOptions.keyframe = Anim::Options::Keyframe();
	nextOptions = std::make_shared<Diag::Options>(*nextOptions);
}

void Chart::setAnimation(const Anim::AnimationPtr &animation)
{
	animator->setAnimation(animation);
}

Diag::Config Chart::getConfig()
{
    return Diag::Config(getSetter());
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
		Draw::drawBackground(layout.boundary.outline(Geom::Size::Square(1)),
		    canvas,
		    actDiagram->getStyle(),
			events.draw.background);

		Draw::drawDiagram(layout.plot,
		    *actDiagram,
		    canvas,
		    Draw::DrawOptions(false, false),
		    actDiagram->getStyle(),
			events.draw);

		actDiagram->getOptions()->legend.get().visit(
			[&](const auto &legend)
		{
			if (legend.value)
				Draw::drawLegend(
					layout.legend,
					*actDiagram,
					events.draw.legend,
					canvas,
					*legend.value,
					legend.weight);
		});

		actDiagram->getOptions()->title.get().visit(
		[&](const auto &title)
		{
			if (title.value.has_value())
				Draw::drawLabel(layout.title,
					*title.value,
					actDiagram->getStyle().title,
					events.draw.title,
					canvas, true, 
					std::max(title.weight * 2 - 1, 0.0));
		});

		Draw::drawMarkerInfo(layout, canvas, *actDiagram);
	}

	if (events.draw.logo->invoke())
	{
		auto filter = *(actDiagram 
				? actDiagram->getStyle() 
				: stylesheet.getDefaultParams()).logo.filter;

		auto logoRect = getLogoBoundary();

		Draw::Logo(canvas).draw(logoRect.pos, logoRect.width(), filter);
	}
}

Geom::Rect Chart::getLogoBoundary() const
{
	auto &logoStyle = (actDiagram 
			? actDiagram->getStyle() 
			: stylesheet.getDefaultParams()).logo;

	auto logoWidth = logoStyle.width->get(
		layout.boundary.size.minSize(), 
		Styles::Sheet::baseFontSize(layout.boundary.size, false));

	auto logoHeight = Draw::Logo::height(logoWidth);

	auto logoPad = logoStyle.toMargin(Geom::Size(logoWidth, logoHeight), 
		Styles::Sheet::baseFontSize(layout.boundary.size, false));

	return Geom::Rect(
		layout.boundary.topRight() - Geom::Point(
			logoPad.right + logoWidth,
			logoPad.bottom + logoHeight),
		Geom::Size(logoWidth, logoHeight));
}

Diag::DiagramPtr Chart::diagram(
    Diag::DiagramOptionsPtr options)
{
	computedStyles = stylesheet.getFullParams(
		options, 
		layout.boundary.size);
	
	return std::make_shared<Diag::Diagram>(table,
	    options,
	    computedStyles);
}

Draw::CoordinateSystem Chart::getCoordSystem() const
{
	const auto &plotArea = layout.plotArea;

	if (actDiagram)
	{
		const auto &options = *actDiagram->getOptions();

		return Draw::CoordinateSystem(plotArea,
			options.angle.get(),
			options.polar.get(),
			actDiagram->keepAspectRatio);
	}
	return Draw::CoordinateSystem(plotArea, 0.0, 
		Math::FuzzyBool(), Math::FuzzyBool());
}

const Diag::Marker *Chart::markerAt(const Geom::Point &point) const
{
	if (!animator || animator->getControl().atIntermediatePosition())
		return nullptr;

	if (actDiagram) 
	{
		const auto &plotArea = layout.plotArea;
		const auto &options = *actDiagram->getOptions();

		Draw::CoordinateSystem coordSys(plotArea,
			options.angle.get(),
			options.polar.get(),
			actDiagram->keepAspectRatio);

		auto originalPos = coordSys.getOriginal(point);

		for (const auto &marker : actDiagram->getMarkers())
		{
			auto drawItem = Draw::DrawItem::create(marker,
				options,
				actDiagram->getStyle(),
				coordSys,
				actDiagram->getMarkers());

			if (drawItem->bounds(originalPos))
				return &marker;
		}
	}
	return nullptr;
}
