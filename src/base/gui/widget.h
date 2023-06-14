#ifndef GUI_WIDGET
#define GUI_WIDGET

#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <string>

#include "base/geom/affinetransform.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

#include "accessories.h"
#include "keys.h"
#include "pointer.h"

namespace GUI
{

class Widget : public std::enable_shared_from_this<Widget>
{
public:
	virtual ~Widget() = default;

	virtual void onPointerDown(const PointerEvent &event) = 0;
	virtual void onPointerUp(const PointerEvent &event) = 0;
	virtual void onPointerMove(const PointerEvent &event) = 0;
	virtual void onWheel(double delta) = 0;
	virtual void onKeyPress(const Key &key,
	    const KeyModifiers &modifiers) = 0;
	virtual void onChanged() const = 0;

	void updateSize(Gfx::ICanvas &canvas, Geom::Size &size);
	void draw(Gfx::ICanvas &canvas);
	[[nodiscard]] Geom::Size getSize() const { return size; }

protected:
	Geom::Size size;

	virtual void setCursor(Cursor cursor) const = 0;
	virtual void onDraw(Gfx::ICanvas &) = 0;
	virtual void onUpdateSize(Gfx::ICanvas &, Geom::Size &) = 0;
};

}

#endif
