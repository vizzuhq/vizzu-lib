#ifndef GUI_LISTWIDGET
#define GUI_LISTWIDGET

#include "widget.h"

namespace GUI
{

class ListWidget : public ContainerWidget
{
public:
	ListWidget(bool horizontal, const Widget *parent);
	void setCellSpacing(double value);
	void setAlign(Align value);
	void onUpdateSize(Gfx::ICanvas &canvas,
	    Geom::Size &size) override;
	bool isHorizontal() const;
	void setHorizontal(bool horizontal);

protected:
	Align align;
	double cellSpacing;
	bool horizontal;
	double mainCoord(const Geom::Point &point) const;
	double subCoord(const Geom::Point &point) const;
	double &mainCoord(Geom::Point &point);
	double &subCoord(Geom::Point &point);
};

}

#endif
