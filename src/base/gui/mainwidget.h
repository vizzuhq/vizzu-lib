#ifndef GUI_MAINWIDGET
#define GUI_MAINWIDGET

#include "widget.h"

namespace GUI
{

class MainWidget : public ContainerWidget
{
public:
	typedef ContainerWidget BaseWidget;

	MainWidget();

	void onUpdateSize(Gfx::ICanvas &info, Geom::Size &size) override;
	DragObjectPtr onPointerDown(
	    const GUI::PointerEvent &event) override;
	bool onPointerUp(const GUI::PointerEvent &event,
	    DragObjectPtr dragObject) override;
	bool onPointerMove(const GUI::PointerEvent &event,
	    DragObjectPtr &dragObject) override;

protected:
	DragObjectPtr dragObject;
};

}

#endif
