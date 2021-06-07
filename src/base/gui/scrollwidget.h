#ifndef GUI_SCROLLWIDGET
#define GUI_SCROLLWIDGET

#include "widget.h"

namespace GUI
{

class ScrollWidget : public WrapperWidget
{
public:
	ScrollWidget(bool horizontal, const Widget *parent);

protected:
	DragObjectPtr startScroll(const Geom::Point &pos);
	void scrollTo(std::weak_ptr<Widget> widget);
	void onUpdateSize(Gfx::ICanvas &canvas, Geom::Size &size) override;
	void onDraw(Gfx::ICanvas &canvas) override;

private:
	bool horizontal;

	DragObjectPtr onMouseDown(const Geom::Point &pos) override;
	bool onMouseMove(const Geom::Point &pos, DragObjectPtr &dragObject) override;
	bool couldScroll() const;
};

}

#endif
