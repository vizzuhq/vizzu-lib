#ifndef GUI_WIDGET
#define GUI_WIDGET

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

#include "pointer.h"

namespace GUI
{

class Widget
{
public:
	virtual ~Widget() = default;

	virtual void onPointerDown(
	    const std::shared_ptr<Gfx::ICanvas> &canvas,
	    const PointerEvent &event) = 0;
	virtual void onPointerUp(
	    const std::shared_ptr<Gfx::ICanvas> &canvas,
	    const PointerEvent &event) = 0;
	virtual void onPointerLeave(
	    const std::shared_ptr<Gfx::ICanvas> &canvas,
	    const PointerEvent &event) = 0;
	virtual void onPointerMove(
	    const std::shared_ptr<Gfx::ICanvas> &canvas,
	    const PointerEvent &event) = 0;
	virtual void onWheel(const std::shared_ptr<Gfx::ICanvas> &canvas,
	    double delta) = 0;
	virtual void onChanged() = 0;
	virtual void setCursor(const std::shared_ptr<Gfx::ICanvas> &,
	    Cursor cursor) const = 0;
	virtual void onDraw(const std::shared_ptr<Gfx::ICanvas> &,
	    bool highResolution) = 0;
	virtual void onUpdateSize(const std::shared_ptr<Gfx::ICanvas> &,
	    Geom::Size) = 0;
	[[nodiscard]] virtual Geom::Size getSize(
	    const std::shared_ptr<Gfx::ICanvas> &) const = 0;
	[[nodiscard]] virtual bool needsUpdate(
	    const std::shared_ptr<Gfx::ICanvas> &) const = 0;
};

}

#endif
