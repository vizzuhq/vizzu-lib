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
#include "chart/generator/diagram.h"
#include "chart/main/layout.h"
#include "chart/main/stylesheet.h"
#include "chart/options/config.h"
#include "chart/rendering/painter/coordinatesystem.h"
#include "data2/table/datatable.h"
#include "events.h"

namespace Vizzu
{

class Chart :
	public Util::EventDispatcher::Sender
{
public:
	typedef std::function<void()> Event;
	Event onChanged;

	Chart();
	void draw(Gfx::ICanvas &canvas) const;
	void setBoundRect(const Geom::Rect &rect, Gfx::ICanvas &info);

	Data::DataTable &getTable() { return table; }
	Diag::OptionsSetterPtr getSetter();
	Styles::Sheet &getStylesheet() { return stylesheet; }
	Styles::Chart &getStyles() { return actStyles; }
	Styles::Chart &getComputedStyles() { return computedStyles; }
	void setStyles(const Styles::Chart &styles) { actStyles = styles; actStyles.setup(); }
	Diag::Options getOptions() { return *nextOptions; }
	void setOptions(const Diag::Options &options) { *nextOptions = options; }
	Diag::DiagramPtr getDiagram() const { return actDiagram; }
	::Anim::Control &getAnimControl() { return *animator; }
	Anim::Options &getAnimOptions() { return nextAnimOptions; }
	Events &getEvents() { return events; }
	Util::EventDispatcher &getEventDispatcher() { return eventDispatcher; }
	Draw::CoordinateSystem getCoordSystem() const;

	Diag::Config getConfig();

	void animate(Event onComplete = Event());
	const Diag::Marker *markerAt(const Geom::Point &point) const;

	Geom::Rect getLogoBoundary() const;

private:
	Layout layout;
	std::shared_ptr<Anim::Animator> animator;
	Data::DataTable table;
	Diag::DiagramPtr actDiagram;
	Diag::DiagramOptionsPtr nextOptions;
	Anim::Options nextAnimOptions;
	Styles::Sheet stylesheet;
	Styles::Chart actStyles;
	Styles::Chart computedStyles;
	Util::EventDispatcher eventDispatcher;
	Events events;

	Diag::DiagramPtr diagram(
	    Diag::DiagramOptionsPtr options);
};

}

#endif
