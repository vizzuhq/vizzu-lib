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

	virtual void onPointerDown(const PointerEvent &event) = 0;
	virtual void onPointerUp(const PointerEvent &event) = 0;
	virtual void onPointerLeave(const PointerEvent &event) = 0;
	virtual void onPointerMove(const PointerEvent &event) = 0;
	virtual void onWheel(double delta) = 0;
	virtual void onChanged() = 0;
	virtual void onDraw(const std::shared_ptr<Gfx::ICanvas> &) = 0;
	virtual void onUpdateSize(const std::shared_ptr<Gfx::ICanvas> &,
	    Geom::Size) = 0;
	[[nodiscard]] virtual bool needsUpdate(
	    const std::shared_ptr<Gfx::ICanvas> &,
	    Geom::Size) const = 0;
};

}

#endif
