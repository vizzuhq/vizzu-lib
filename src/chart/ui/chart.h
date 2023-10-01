#ifndef CHART_UI_CHART_H
#define CHART_UI_CHART_H

#include <optional>

#include "base/gui/pointer.h"
#include "base/gui/scheduler.h"
#include "base/gui/widget.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/chart.h"

namespace Vizzu::UI
{

class ChartWidget : public GUI::Widget
{
public:
	std::function<void(void)> doChange;
	std::function<void(const std::shared_ptr<Gfx::ICanvas> &,
	    GUI::Cursor)>
	    doSetCursor;
	std::function<void(const std::string &)> openUrl;

	explicit ChartWidget(GUI::Scheduler &scheduler);
	~ChartWidget() override;

	void onPointerDown(const std::shared_ptr<Gfx::ICanvas> &,
	    const GUI::PointerEvent &event) override;
	void onPointerMove(const std::shared_ptr<Gfx::ICanvas> &,
	    const GUI::PointerEvent &event) override;
	void onPointerUp(const std::shared_ptr<Gfx::ICanvas> &,
	    const GUI::PointerEvent &event) override;
	void onPointerLeave(const std::shared_ptr<Gfx::ICanvas> &,
	    const GUI::PointerEvent &event) override;
	void onWheel(const std::shared_ptr<Gfx::ICanvas> &,
	    double delta) override;
	void setCursor(const std::shared_ptr<Gfx::ICanvas> &,
	    GUI::Cursor cursor) const override;
	void onChanged() override;
	void onDraw(const std::shared_ptr<Gfx::ICanvas> &) override;
	void onUpdateSize(const std::shared_ptr<Gfx::ICanvas> &,
	    Geom::Size size) override;

	[[nodiscard]] Geom::Size getSize(
	    const std::shared_ptr<Gfx::ICanvas> &) const override;

	[[nodiscard]] Chart &getChart() { return chart; }

	[[nodiscard]] bool needsUpdate(
	    const std::shared_ptr<Gfx::ICanvas> &) const final
	{
		return needUpdate;
	}

private:
	Chart chart;
	GUI::PointerEvent pointerEvent;
	std::reference_wrapper<GUI::Scheduler> scheduler;
	Util::EventDispatcher::event_ptr onClick;
	Util::EventDispatcher::event_ptr onPointerMoveEvent;
	Util::EventDispatcher::event_ptr onPointerOnEvent;
	Util::EventDispatcher::event_ptr onWheelEvent;
	Util::EventDispatcher::event_ptr onPointerDownEvent;
	Util::EventDispatcher::event_ptr onPointerUpEvent;
	bool unprocessedPointerMove{};
	bool unprocessedPointerLeave{};
	std::optional<int64_t> trackedMarkerId;
	std::optional<int64_t> reportedMarkerId;
	bool needUpdate{true};

	void updateCursor(const std::shared_ptr<Gfx::ICanvas> &);
	void trackMarker();
	const Gen::Marker *getMarkerAt(const Geom::Point &pos);
};

}

#endif
