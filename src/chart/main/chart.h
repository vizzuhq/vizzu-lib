#ifndef VIZZU_CHART_H
#define VIZZU_CHART_H

#include <functional>
#include <memory>
#include <string>

#include "base/anim/control.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/animator/animator.h"
#include "chart/generator/plot.h"
#include "chart/main/layout.h"
#include "chart/main/stylesheet.h"
#include "chart/options/config.h"
#include "chart/rendering/renderedchart.h"
#include "data/table/datatable.h"

#include "events.h"

namespace Vizzu
{

class Chart
{
public:
	using Event = std::function<void()>;
	using OnComplete = std::function<void(bool)>;

	Event onChanged;

	Chart();
	Chart(Chart &&) noexcept = delete;
	void draw(Gfx::ICanvas &canvas);
	void setBoundRect(const Geom::Rect &rect);

	Data::DataTable &getTable() { return table; }
	Styles::Sheet &getStylesheet() { return stylesheet; }
	Styles::Chart &getStyles() { return actStyles; }
	[[nodiscard]] const Styles::Chart &getComputedStyles() const
	{
		return computedStyles;
	}
	void setStyles(const Styles::Chart &styles)
	{
		actStyles = styles;
		actStyles.setup();
	}
	Gen::Options &getOptions() { return *nextOptions; }
	void setOptions(const Gen::Options &options)
	{
		*nextOptions = options;
	}
	[[nodiscard]] const Gen::PlotPtr &getPlot() const
	{
		return actPlot;
	}
	::Anim::Control &getAnimControl()
	{
		return animator->getControl();
	}
	Anim::AnimationPtr getAnimation()
	{
		return animator->getActAnimation();
	}
	Anim::Options &getAnimOptions() { return nextAnimOptions; }
	[[nodiscard]] const Layout &getLayout() const { return layout; }
	Util::EventDispatcher &getEventDispatcher()
	{
		return eventDispatcher;
	}
	[[nodiscard]] const Draw::RenderedChart &getRenderedChart() const
	{
		return renderedChart;
	}

	Gen::Config getConfig();

	void animate(const OnComplete &onComplete = OnComplete());
	void setKeyframe();
	void setAnimation(const Anim::AnimationPtr &animation);

private:
	Layout layout;
	std::shared_ptr<Anim::Animator> animator;
	Data::DataTable table;
	Gen::PlotPtr actPlot;
	Gen::PlotOptionsPtr nextOptions;
	Gen::Options prevOptions;
	Anim::Options nextAnimOptions;
	Styles::Sheet stylesheet;
	Styles::Chart actStyles;
	Styles::Chart prevStyles;
	Styles::Chart computedStyles;
	Util::EventDispatcher eventDispatcher;
	Draw::RenderedChart renderedChart;
	Events events;

	Gen::PlotPtr plot(const Gen::PlotOptionsPtr &options);
};

}

#endif
