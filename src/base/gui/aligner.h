#ifndef GUI_ALIGNER
#define GUI_ALIGNER

#include "accessories.h"
#include "widget.h"

namespace GUI
{

class Aligner : public WrapperWidget
{
public:
	Aligner(bool horizontal,
			Align align,
			double fixSize,
			Widget *widget);
private:
	bool horizontal;
	Align align;
	double fixSize;
	void onUpdateSize(Gfx::ICanvas &info, Geom::Size &size) override;
};

}

#endif
