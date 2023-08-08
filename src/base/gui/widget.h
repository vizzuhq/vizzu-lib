#ifndef GUI_WIDGET
#define GUI_WIDGET

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

#include "keys.h"
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
	virtual void onKeyPress(const Key &key,
	    const KeyModifiers &modifiers) = 0;
	virtual void onChanged() const = 0;
	virtual void setCursor(Cursor cursor) const = 0;
	virtual void onDraw(Gfx::ICanvas &) = 0;
	virtual void onUpdateSize(Gfx::ICanvas &, Geom::Size) = 0;
	[[nodiscard]] virtual Geom::Size getSize() const = 0;
};

}

#endif
