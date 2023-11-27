#include "chart.h"

#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/drawlegend.h"
#include "chart/rendering/drawmarkerinfo.h"
#include "chart/rendering/drawplot.h"
#include "chart/rendering/logo.h"
#include "data/datacube/datacube.h"

namespace Vizzu
{

Chart::Chart() :
    animator(std::make_shared<Anim::Animator>()),
    stylesheet(Styles::Chart::def()),
    computedStyles(stylesheet.getDefaultParams()),
    events(*this)
{
	stylesheet.setActiveParams(actStyles);
	nextOptions = std::make_shared<Gen::Options>();

	animator->onDraw.attach(
	    [this](const Gen::PlotPtr &actPlot)
	    {
		    this->actPlot = actPlot;
		    if (onChanged) onChanged();
	    });
	animator->onProgress.attach(
	    [this]()
	    {
		    events.animation.update->invoke(
		        Events::OnUpdateEvent(animator->getControl()));
	    });
	animator->onBegin = [this]()
	{
		events.animation.begin->invoke();
	};
	animator->onComplete = [this]()
	{
		events.animation.complete->invoke();
	};
}

void Chart::setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info)
{
	if (actPlot) {
		actPlot->getStyle().setup();
		layout.setBoundary(rect, *actPlot, info);
	}
	else {
		layout.setBoundary(rect, info);
	}
}

void Chart::animate(const OnComplete &onComplete)
{
	auto f = [this, onComplete](const Gen::PlotPtr &plot, bool ok)
	{
		actPlot = plot;
		if (ok) {
			prevOptions = *nextOptions;
			prevStyles = actStyles;
		}
		else {
			*nextOptions = prevOptions;
			actStyles = prevStyles;
			computedStyles = plot->getStyle();
		}
		if (onComplete) onComplete(ok);
	};
	animator->animate(nextAnimOptions.control, f);
	nextAnimOptions = Anim::Options();
	nextOptions = std::make_shared<Gen::Options>(*nextOptions);
}

void Chart::setKeyframe()
{
	animator->addKeyframe(plot(nextOptions),
	    nextAnimOptions.keyframe);
	nextAnimOptions.keyframe = Anim::Options::Keyframe();
	nextOptions = std::make_shared<Gen::Options>(*nextOptions);
}

void Chart::setAnimation(const Anim::AnimationPtr &animation)
{
	animator->setAnimation(animation);
}

Gen::Config Chart::getConfig() { return Gen::Config{getSetter()}; }

Gen::OptionsSetter Chart::getSetter()
{
	Gen::OptionsSetter setter(*nextOptions);
	setter.setTable(&table);
	return setter;
}

template <class T>
void Chart::drawHeading(const Draw::DrawingContext &context,
    const Gen::Options::Heading &option,
    const Styles::Label &style,
    const Geom::Rect &layout,
    const Util::EventDispatcher::event_ptr &event)
{
	option.visit(
	    [&](int, const auto &weighted)
	    {
		    if (weighted.value.has_value()) {
			    Draw::DrawLabel(context,
			        Geom::TransformedRect::fromRect(layout),
			        *weighted.value,
			        style,
			        event,
			        std::make_unique<T>(*weighted.value),
			        Draw::DrawLabel::Options(true,
			            std::max(weighted.weight * 2 - 1, 0.0)));
		    }
	    });
}

void Chart::draw(Gfx::ICanvas &canvas)
{
	if (actPlot
	    && (!events.draw.begin
	        || events.draw.begin->invoke(
	            Util::EventDispatcher::Params{}))) {

		auto coordSys = getCoordSystem();

		Draw::RenderedChart rendered(coordSys, actPlot);

		Draw::DrawingContext context(canvas,
		    layout,
		    events,
		    *actPlot,
		    coordSys,
		    rendered);

		Draw::DrawBackground(context,
		    layout.boundary.outline(Geom::Size::Square(1)),
		    actPlot->getStyle(),
		    events.draw.background,
		    std::make_unique<Events::Targets::Root>());

		Draw::DrawPlot{context};

		actPlot->getOptions()->legend.visit(
		    [&](int, const auto &legend)
		    {
			    if (*legend.value)
				    Draw::DrawLegend(context,
				        Gen::Options::toChannel(**legend.value),
				        legend.weight);
		    });

		drawHeading<Events::Targets::ChartTitle>(context,
		    actPlot->getOptions()->title,
		    actPlot->getStyle().title,
		    layout.title,
		    events.draw.title);

		drawHeading<Events::Targets::ChartSubtitle>(context,
		    actPlot->getOptions()->subtitle,
		    actPlot->getStyle().subtitle,
		    layout.subtitle,
		    events.draw.subtitle);

		drawHeading<Events::Targets::ChartCaption>(context,
		    actPlot->getOptions()->caption,
		    actPlot->getStyle().caption,
		    layout.caption,
		    events.draw.caption);

		Draw::DrawMarkerInfo(layout, canvas, *actPlot);

		renderedChart = std::move(rendered);
	}

	auto logoRect = getLogoBoundary();
	if (auto logoElement = std::make_unique<Events::Targets::Logo>();
	    events.draw.logo->invoke(Events::OnRectDrawEvent(*logoElement,
	        {logoRect, false}))) {
		auto filter = *(actPlot ? actPlot->getStyle()
		                        : stylesheet.getDefaultParams())
		                   .logo.filter;

		Draw::Logo(canvas).draw(logoRect.pos,
		    logoRect.width(),
		    filter);

		renderedChart.emplace(
		    Geom::TransformedRect::fromRect(logoRect),
		    std::move(logoElement));
	}

	if (events.draw.complete)
		events.draw.complete->invoke(Util::EventDispatcher::Params{});
}

Geom::Rect Chart::getLogoBoundary() const
{
	const auto &logoStyle = (actPlot ? actPlot->getStyle()
	                                 : stylesheet.getDefaultParams())
	                            .logo;

	auto logoWidth =
	    logoStyle.width->get(layout.boundary.size.minSize(),
	        Styles::Sheet::baseFontSize(layout.boundary.size, false));

	auto logoHeight = Draw::Logo::height(logoWidth);

	auto logoPad =
	    logoStyle.toMargin(Geom::Size{logoWidth, logoHeight},
	        Styles::Sheet::baseFontSize(layout.boundary.size, false));

	return {layout.boundary.topRight()
	            - Geom::Point{logoPad.right + logoWidth,
	                logoPad.bottom + logoHeight},
	    Geom::Size{logoWidth, logoHeight}};
}

Gen::PlotPtr Chart::plot(const Gen::PlotOptionsPtr &options)
{
	options->setAutoParameters();

	computedStyles =
	    stylesheet.getFullParams(options, layout.boundary.size);

	return std::make_shared<Gen::Plot>(table,
	    options,
	    computedStyles,
	    false);
}

Draw::CoordinateSystem Chart::getCoordSystem() const
{
	if (actPlot) {
		const auto &rootStyle = actPlot->getStyle();

		auto plotArea = rootStyle.plot.contentRect(layout.plot,
		    rootStyle.calculatedSize());

		const auto &options = *actPlot->getOptions();

		return {plotArea,
		    options.angle,
		    options.coordSystem,
		    actPlot->keepAspectRatio};
	}
	return {layout.plotArea,
	    0.0,
	    ::Anim::Interpolated<Gen::CoordSystem>{
	        Gen::CoordSystem::cartesian},
	    Math::FuzzyBool()};
}

}