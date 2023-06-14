#ifndef CHART_UI_CHART_H
#define CHART_UI_CHART_H

#include "base/gui/keys.h"
#include "base/gui/pointer.h"
#include "base/gui/scheduler.h"
#include "base/gui/widget.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/chart.h"

namespace Vizzu
{
namespace UI
{

class ChartWidget : public GUI::Widget
{
public:
	std::function<void(void)> doChange;
	std::function<void(GUI::Cursor)> doSetCursor;
	std::function<void(const std::string &)> openUrl;
	std::function<Gfx::ICanvasPtr(const std::string &)> getCanvas;

	ChartWidget(GUI::SchedulerPtr scheduler);
	~ChartWidget() override;

	void onPointerDown(
	    const GUI::PointerEvent &event) override;
	void onPointerMove(const GUI::PointerEvent &event) override;
	void onPointerUp(const GUI::PointerEvent &event) override;
	void onPointerLeave(const GUI::PointerEvent &event) override;
	void onWheel(double delta) override;
	void onKeyPress(const GUI::Key &,
	    const GUI::KeyModifiers &) override {}
	void setCursor(GUI::Cursor cursor) const override;
	void onChanged() const override;
	void onDraw(Gfx::ICanvas &) override;
	void onUpdateSize(Gfx::ICanvas &info, Geom::Size size) override;


	[[nodiscard]] Geom::Size getSize() const override;

	[[nodiscard]] Chart &getChart() { return chart; }

private:
	Chart chart;
	bool selectionEnabled;
	GUI::PointerEvent pointerEvent;
	GUI::SchedulerPtr scheduler;
	Util::EventDispatcher::event_ptr onClick;
	Util::EventDispatcher::event_ptr onPointerMoveEvent;
	Util::EventDispatcher::event_ptr onPointerOnEvent;
	Util::EventDispatcher::event_ptr onWheelEvent;
	Util::EventDispatcher::event_ptr onPointerDownEvent;
	Util::EventDispatcher::event_ptr onPointerUpEvent;
	bool unprocessedPointerMove;
	bool unprocessedPointerLeave;
	int64_t trackedMarkerId;
	int64_t reportedMarkerId;

	void updateCursor();
	void trackMarker();
};

}
}

#endif
