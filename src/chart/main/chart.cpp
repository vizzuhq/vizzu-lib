#include "chart.h"

#include "chart/generator/plot.h"
#include "chart/rendering/drawchart.h"

namespace Vizzu
{

Chart::Chart() :
    animator(std::make_shared<Anim::Animator>()),
    stylesheet(Styles::Chart::def()),
    computedStyles(stylesheet.getDefaultParams()),
    events(getEventDispatcher())
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

void Chart::setBoundRect(const Geom::Rect &rect)
{
	if (actPlot) {
		actPlot->getStyle().setup();
		layout.setBoundary(rect,
		    actPlot->getStyle(),
		    actPlot->getOptions());
	}
	else {
		layout.setBoundary(rect,
		    stylesheet.getDefaultParams(),
		    nullptr);
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

Gen::Config Chart::getConfig()
{
	return Gen::Config{getOptions(), table};
}

void Chart::draw(Gfx::ICanvas &canvas)
{
	renderedChart = Draw::RenderedChart{
	    actPlot ? Draw::CoordinateSystem{layout.plotArea,
	        actPlot->getOptions()->angle,
	        actPlot->getOptions()->coordSystem,
	        actPlot->keepAspectRatio}
	            : Draw::CoordinateSystem{layout.plotArea},
	    actPlot};

	Draw::DrawChart{Draw::DrawingContext{actPlot,
	                    actPlot->getOptions().get(),
	                    renderedChart,
	                    renderedChart.getCoordSys(),
	                    actPlot ? actPlot->getStyle()
	                            : stylesheet.getDefaultParams(),
	                    events}}
	    .draw(canvas, layout);
}

Gen::PlotPtr Chart::plot(const Gen::PlotOptionsPtr &options)
{
	options->setAutoParameters();

	auto res = std::make_shared<Gen::Plot>(table,
	    options,
	    stylesheet.getFullParams(options, layout.boundary.size),
	    false);

	Styles::Sheet::setAfterStyles(*res, layout.boundary.size);

	computedStyles = res->getStyle();

	return res;
}

}