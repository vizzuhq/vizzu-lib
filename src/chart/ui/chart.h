#ifndef CHART_UI_CHART_H
#define CHART_UI_CHART_H

#include "base/gui/keys.h"
#include "base/gui/listwidget.h"
#include "base/gui/mainwidget.h"
#include "base/gui/mouse.h"
#include "base/gui/scheduler.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/chart.h"

namespace Vizzu
{
namespace UI
{

class ChartWidget :
    public GUI::MainWidget
{
public:
	std::function<void(void)> doChange;
	std::function<void(GUI::Cursor)> setMouseCursor;
	std::function<void(const std::string &)> openUrl;
	std::function<Gfx::ICanvasPtr(const std::string &)> getCanvas;

	ChartWidget(GUI::SchedulerPtr scheduler);
	~ChartWidget() override;

	GUI::DragObjectPtr onMouseDown(
		const Geom::Point &pos) override;
	bool onMouseMove(const Geom::Point &pos,
	    GUI::DragObjectPtr &dragObject) override;
	bool onMouseUp(const Geom::Point &pos,
	    GUI::DragObjectPtr dragObject) override;
	void onMouseLeave();
	bool onMouseWheel(double delta) override;

	Chart &getChart() { return *chart; }

private:
	std::shared_ptr<Chart> chart;
	bool selectionEnabled;
	Geom::Point mousePos;
	GUI::SchedulerPtr scheduler;
	Util::EventDispatcher::event_ptr onClick;
	Util::EventDispatcher::event_ptr onMouseMoveEvent;
	Util::EventDispatcher::event_ptr onMouseOnEvent;
	Util::EventDispatcher::event_ptr onMouseWheelEvent;
	Util::EventDispatcher::event_ptr onMouseDownEvent;
	Util::EventDispatcher::event_ptr onMouseUpEvent;
	bool unprocessedMouseMove;
	bool unprocessedMouseLeave;
	int64_t trackedMarkerId;
	int64_t reportedMarkerId;

	void setCursor(GUI::Cursor cursor) const override;
	void onChanged() const override;
	void onDraw(Gfx::ICanvas &) override;
	void onUpdateSize(Gfx::ICanvas &info,
	    Geom::Size &size) override;
	void updateMouseCursor();
	void trackMarker();
};

}
}

#endif
