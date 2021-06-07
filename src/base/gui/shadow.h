#ifndef GUI_SHADOW
#define GUI_SHADOW

#include "widget.h"

namespace GUI
{

class Shadow
{
public:
	enum class Side : uint32_t { Left, Right };

	Shadow(Widget *parent, Side side);
	void draw(Gfx::ICanvas &canvas);

private:
	Widget *parent;
	Side side;
};

}

#endif
