#include "aligner.h"

using namespace GUI;

Aligner::Aligner(bool horizontal,
				 Align align,
				 double fixSize,
				 Widget *widget)
	: WrapperWidget(widget),
	  horizontal(horizontal),
	  align(align),
	  fixSize(fixSize)
{}

void Aligner::onUpdateSize(Gfx::ICanvas &info, Geom::Size &size)
{
	if (onlyChild.lock())
	{
		Geom::Size childSize;
		onlyChild.lock()->updateSize(info, childSize);

		auto childPos = boundary.pos;

		if (horizontal) {
			if (align == Align::Max)
				childPos.x += fixSize - childSize.x;
			if (align == Align::Center)
				childPos.x += (fixSize - childSize.x) / 2;
		} else {
			if (align == Align::Max)
				childPos.y += fixSize - childSize.y;
			if (align == Align::Center)
				childPos.y += (fixSize - childSize.y) / 2;
		}

		if (horizontal) size = Geom::Size(fixSize, childSize.y);
		else size = Geom::Size(childSize.x, fixSize);
	}
	else
	{
		if (horizontal) size = Geom::Size(fixSize, 0);
		else size = Geom::Size(0, fixSize);
	}
}
