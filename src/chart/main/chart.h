#ifndef VIZZU_CHART_H
#define VIZZU_CHART_H

#include <functional>
#include <memory>
#include <string>

#include "base/anim/control.h"
#include "base/gfx/canvas.h"
#include "base/gui/scheduler.h"
#include "base/util/eventdispatcher.h"
#include "chart/animator/animator.h"
#include "chart/generator/plot.h"
#include "chart/main/layout.h"
#include "chart/main/stylesheet.h"
#include "chart/options/config.h"
#include "chart/rendering/painter/coordinatesystem.h"
#include "chart/rendering/renderedchart.h"
#include "data/table/datatable.h"

#include "events.h"

namespace Vizzu
{

class Chart : public Util::EventTarget
{
public:
	typedef std::function<void()> Event;
	typedef std::function<void(bool)> OnComplete;

	Event onChanged;

	Chart();
	void draw(Gfx::ICanvas &canvas);
	void setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info);

	Data::DataTable &getTable() { return table; }
	Gen::OptionsSetterPtr getSetter();
	Styles::Sheet &getStylesheet() { return stylesheet; }
	Styles::Chart &getStyles() { return actStyles; }
	Styles::Chart &getComputedStyles() { return computedStyles; }
	void setStyles(const Styles::Chart &styles)
	{
		actStyles = styles;
		actStyles.setup();
	}
	Gen::Options getOptions() { return *nextOptions; }
	void setOptions(const Gen::Options &options)
	{
		*nextOptions = options;
	}
	Gen::PlotPtr getPlot() const { return actPlot; }
	::Anim::Control &getAnimControl()
	{
		return animator->getControl();
	}
	Anim::AnimationPtr getAnimation()
	{
		return animator->getActAnimation();
	}
	Anim::Options &getAnimOptions() { return nextAnimOptions; }
	Events &getEvents() { return events; }
	const Layout &getLayout() const { return layout; }
	Util::EventDispatcher &getEventDispatcher()
	{
		return eventDispatcher;
	}
	Draw::CoordinateSystem getCoordSystem() const;

	Gen::Config getConfig();

	void animate(OnComplete onComplete = OnComplete());
	void setKeyframe();
	void setAnimation(const Anim::AnimationPtr &animation);
	Gen::Marker *markerAt(const Geom::Point &point) const;
	const Gen::Marker *markerByIndex(size_t index) const;
	Geom::Rect getLogoBoundary() const;

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

	Gen::PlotPtr plot(Gen::PlotOptionsPtr options);
};

}

#endif
