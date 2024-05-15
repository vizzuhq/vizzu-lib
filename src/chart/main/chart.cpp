#include "chart.h"

#include "chart/generator/plot.h"
#include "chart/rendering/drawchart.h"

namespace Vizzu
{

Chart::Chart() :
    nextOptions(std::make_shared<Gen::Options>()),
    stylesheet(Styles::Chart::def(), actStyles),
    computedStyles(stylesheet.getDefaultParams()),
    events(eventDispatcher),
    animator(*events.animation.begin, *events.animation.complete)
{
	animator.onDraw.attach(
	    [this](const Gen::PlotPtr &actPlot)
	    {
		    this->actPlot = actPlot;
		    onChanged();
	    });
	animator.onProgress.attach(
	    [this]
	    {
		    events.animation.update->invoke(
		        Events::OnUpdateEvent(animator.getControl()));
	    });
}

void Chart::setBoundRect(const Geom::Rect &rect)
{
	if (actPlot) {
		auto &style = actPlot->getStyle();
		style.setup();
		if (!actStyles.fontSize)
			computedStyles.fontSize = style.fontSize = Gfx::Length{
			    Styles::Sheet::baseFontSize(rect.size, true)};
		layout.setBoundary(rect, style, actPlot->getOptions());
	}
	else {
		layout.setBoundary(rect,
		    stylesheet.getDefaultParams(),
		    nullptr);
	}
}

void Chart::animate(Anim::Animation::OnComplete &&onComplete)
{
	onComplete.attach(
	    [this](const Gen::PlotPtr &plot, const bool &ok)
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
	    });

	animator.animate(nextAnimOptions.control, std::move(onComplete));
	nextAnimOptions = Anim::Options();
	nextOptions = std::make_shared<Gen::Options>(*nextOptions);
}

void Chart::setKeyframe()
{
	animator.addKeyframe(plot(nextOptions), nextAnimOptions.keyframe);
	nextAnimOptions.keyframe = Anim::Options::Keyframe();
	nextOptions = std::make_shared<Gen::Options>(*nextOptions);
}

void Chart::setAnimation(const Anim::AnimationPtr &animation)
{
	animator.setAnimation(animation);
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

	Draw::DrawChart{
	    Draw::DrawingContext{actPlot,
	        actPlot ? actPlot->getOptions().get() : nullptr,
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
	    stylesheet.getFullParams(options, layout.boundary.size));

	Styles::Sheet::setAfterStyles(*res, layout.boundary.size);

	computedStyles = res->getStyle();

	return res;
}

}