#ifndef CHART_UI_CHART_H
#define CHART_UI_CHART_H

#include "base/gfx/pathsampler.h"
#include "base/gui/pointer.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/chart.h"

namespace Vizzu::UI
{

class ChartWidget
{
public:
	Util::Event<const std::string> openUrl;

	explicit ChartWidget();

	void onPointerDown(const GUI::PointerEvent &event) const;
	void onPointerMove(const GUI::PointerEvent &event) const;
	void onPointerUp(const GUI::PointerEvent &event) const;
	void onPointerLeave(const GUI::PointerEvent &event) const;
	void onWheel(double delta) const;
	void onDraw(const std::shared_ptr<Gfx::ICanvas> &);
	void onUpdateSize(Geom::Size size);

	[[nodiscard]] Chart &getChart() { return chart; }

private:
	Chart chart;
	Util::EventDispatcher::event_ptr onClick;
	Util::EventDispatcher::event_ptr onPointerMoveEvent;
	Util::EventDispatcher::event_ptr onWheelEvent;
	Util::EventDispatcher::event_ptr onPointerDownEvent;
	Util::EventDispatcher::event_ptr onPointerUpEvent;
	Util::EventDispatcher::event_ptr onPointerLeaveEvent;
};

}

#endif
