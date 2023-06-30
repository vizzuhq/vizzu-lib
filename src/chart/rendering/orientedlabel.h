#ifndef DRAWORIENTEDLABEL_H
#define DRAWORIENTEDLABEL_H

#include "base/geom/transformedrect.h"
#include "chart/main/events.h"
#include "chart/rendering/drawlabel.h"

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class OrientedLabel 
{
public:
	OrientedLabel(const std::string &text) : text(text) {} 
	Geom::TransformedRect rect;
	Geom::Rect contentRect;
	const std::string &text;
};

class OrientedLabelRenderer : private DrawingContext
{
public:
	OrientedLabelRenderer(const DrawingContext &context);

	OrientedLabel create(
	    const std::string &text,
	    const Geom::Line &labelPos,
	    const Styles::OrientedLabel &labelStyle,
	    double centered
	) const;

	void render(
	    const OrientedLabel &label,
	    const Gfx::Color &textColor,
	    const Gfx::Color &bgColor) const;
};

}
}

#endif
