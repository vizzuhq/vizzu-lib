#include "chart.h"

#include "chart/rendering/drawchart.h"
#include "data/datacube/datacube.h"

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

void Chart::setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info)
{
	if (actPlot) {
		actPlot->getStyle().setup();
		layout.setBoundary(rect,
		    actPlot->getStyle(),
		    actPlot->getOptions(),
		    info);
	}
	else {
		layout.setBoundary(rect,
		    stylesheet.getDefaultParams(),
		    nullptr,
		    info);
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

void Chart::draw(Gfx::ICanvas &canvas, bool highResolution)
{
	static_cast<Draw::Painter *>(canvas.getPainter())
	    ->setResMode(highResolution ? Draw::ResolutionMode::High
	                                : Draw::ResolutionMode::Low);

	Draw::DrawChart{Draw::DrawingContext{canvas,
	                    layout,
	                    events,
	                    actPlot,
	                    actPlot ? actPlot->getStyle()
	                            : stylesheet.getDefaultParams(),
	                    renderedChart}}
	    .draw();
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

}