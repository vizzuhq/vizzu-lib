#ifndef CHART_UI_CHART_H
#define CHART_UI_CHART_H

#include <optional>

#include "base/gui/pointer.h"
#include "base/gui/widget.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/chart.h"

namespace Vizzu::UI
{

class ChartWidget : public GUI::Widget
{
public:
	std::function<void()> doChange;
	std::function<void(const std::string &)> openUrl;

	explicit ChartWidget();
	~ChartWidget() override;

	void onPointerDown(const GUI::PointerEvent &event) override;
	void onPointerMove(const GUI::PointerEvent &event) override;
	void onPointerUp(const GUI::PointerEvent &event) override;
	void onPointerLeave(const GUI::PointerEvent &event) override;
	void onWheel(double delta) override;
	void onChanged() override;
	void onDraw(const std::shared_ptr<Gfx::ICanvas> &) override;
	void onUpdateSize(Geom::Size size) override;

	[[nodiscard]] Chart &getChart() { return chart; }

	[[nodiscard]] bool needsUpdate(Geom::Size size) const final
	{
		return needUpdate || chart.getLayout().boundary.size != size;
	}

private:
	Chart chart;
	Util::EventDispatcher::event_ptr onClick;
	Util::EventDispatcher::event_ptr onPointerMoveEvent;
	Util::EventDispatcher::event_ptr onWheelEvent;
	Util::EventDispatcher::event_ptr onPointerDownEvent;
	Util::EventDispatcher::event_ptr onPointerUpEvent;
	Util::EventDispatcher::event_ptr onPointerLeaveEvent;
	bool needUpdate{true};
};

}

#endif
