#include "chart.h"

#include "chart/options/advancedoptions.h"
#include "chart/rendering/drawbackground.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/drawlegend.h"
#include "chart/rendering/drawmarkerinfo.h"
#include "chart/rendering/drawplot.h"
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
	nextOptions = std::make_shared<Gen::Options>();

	animator->onDraw.attach(
	    [&](const Gen::PlotPtr &actPlot)
	    {
		    this->actPlot = actPlot;
		    if (onChanged) onChanged();
	    });
	animator->onProgress.attach(
	    [&]()
	    {
		    events.animation.update->invoke(
		        Events::OnUpdateParam(animator->getControl()));
	    });
	animator->onBegin = [&]()
	{
		events.animation.begin->invoke(
		    Util::EventDispatcher::Params{nullptr});
	};
	animator->onComplete = [&]()
	{
		events.animation.complete->invoke(
		    Util::EventDispatcher::Params{nullptr});
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
	auto f = [=, this](const Gen::PlotPtr &plot, bool ok)
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

Gen::Config Chart::getConfig() { return {getSetter()}; }

Gen::OptionsSetterPtr Chart::getSetter()
{
	auto setter =
	    std::make_shared<Gen::OrientationSelector>(*nextOptions);
	setter->setTable(&table);
	return setter;
}

void Chart::draw(Gfx::ICanvas &canvas)
{
	if (actPlot
	    && (!events.draw.begin
	        || events.draw.begin->invoke(
	            Util::EventDispatcher::Params{}))) {
		Draw::DrawingContext context(canvas,
		    layout,
		    events,
		    *actPlot);

		auto rootElement = std::make_unique<Events::Targets::Root>();

		Draw::DrawBackground(context,
		    layout.boundary.outline(Geom::Size::Square(1)),
		    actPlot->getStyle(),
		    events.draw.background,
		    std::move(rootElement));

		const Draw::DrawPlot drawPlot(context);

		actPlot->getOptions()->legend.visit(
		    [&](int, const auto &legend)
		    {
			    if (legend.value)
				    Draw::DrawLegend(context,
				        *legend.value,
				        legend.weight);
		    });

		actPlot->getOptions()->title.visit(
		    [&](int, const auto &title)
		    {
			    if (title.value.has_value())
				{
				    auto titleElement = std::make_unique
						<Events::Targets::ChartTitle>(*title.value);

				    Draw::DrawLabel(
				        context,
				        Geom::TransformedRect(layout.title),
				        *title.value,
				        actPlot->getStyle().title,
				        events.draw.title,
				        std::move(titleElement),
				        Draw::DrawLabel::Options(true,
				            std::max(title.weight * 2 - 1, 0.0)));
				}
		    });

		Draw::DrawMarkerInfo(layout, canvas, *actPlot);

		renderedChart = std::move(*context.renderedChart);
 	}

	auto logoElement = std::make_unique<Events::Targets::Logo>();
	auto logoRect = getLogoBoundary();
	if (events.draw.logo->invoke(Events::OnRectDrawParam(
	        *logoElement, logoRect)))
	{
		auto filter = *(actPlot ? actPlot->getStyle()
		                        : stylesheet.getDefaultParams())
		                   .logo.filter;

		Draw::Logo(canvas).draw(logoRect.pos,
		    logoRect.width(),
		    filter);

		renderedChart.emplace(
			Geom::TransformedRect(logoRect), std::move(logoElement));
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
	    logoStyle.toMargin(Geom::Size(logoWidth, logoHeight),
	        Styles::Sheet::baseFontSize(layout.boundary.size, false));

	return {layout.boundary.topRight()
	            - Geom::Point(logoPad.right + logoWidth,
	                logoPad.bottom + logoHeight),
	    Geom::Size(logoWidth, logoHeight)};
}

Gen::PlotPtr Chart::plot(const Gen::PlotOptionsPtr &options)
{
	computedStyles =
	    stylesheet.getFullParams(options, layout.boundary.size);

	return std::make_shared<Gen::Plot>(table,
	    options,
	    computedStyles);
}

Draw::CoordinateSystem Chart::getCoordSystem() const
{
	const auto &plotArea = layout.plotArea;

	if (actPlot) {
		const auto &options = *actPlot->getOptions();

		return {plotArea,
		    options.angle,
		    options.coordSystem,
		    actPlot->keepAspectRatio};
	}
	return {plotArea,
	    0.0,
	    ::Anim::Interpolated<Gen::CoordSystem>{
	        Gen::CoordSystem::cartesian},
	    Math::FuzzyBool()};
}

Gen::Marker *Chart::markerAt(const Geom::Point &/*point*/) const
{
/*	if (actPlot) {
		const auto &plotArea = layout.plotArea;
		const auto &options = *actPlot->getOptions();

		const Draw::CoordinateSystem coordSys(plotArea,
		    options.angle,
		    options.coordSystem,
		    actPlot->keepAspectRatio);

		auto originalPos = coordSys.getOriginal(point);

		for (auto &marker : actPlot->getMarkers()) {
			auto drawItem =
			    Draw::AbstractMarker::createInterpolated(marker,
			        options,
			        actPlot->getStyle(),
			        coordSys,
			        actPlot->getMarkers(),
			        0);

			if (drawItem.bounds(originalPos)) return &marker;
		}
	}
*/	return nullptr;
}

const Gen::Marker *Chart::markerByIndex(size_t index) const
{
	if (actPlot) {
		auto &markers = actPlot->getMarkers();
		if (index < markers.size()) return &markers[index];
	}
	return nullptr;
}
