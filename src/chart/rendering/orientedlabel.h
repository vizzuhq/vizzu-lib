#ifndef DRAWORIENTEDLABEL_H
#define DRAWORIENTEDLABEL_H

#include "base/geom/transformedrect.h"
#include "chart/rendering/drawlabel.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class OrientedLabel
{
public:
	explicit OrientedLabel(const std::string &text) : text(text) {}
	Geom::TransformedRect rect;
	Geom::Rect contentRect;
	const std::string &text;
};

class OrientedLabelRenderer : private DrawingContext
{
public:
	explicit OrientedLabelRenderer(const DrawingContext &context);

	[[nodiscard]] OrientedLabel create(const std::string &text,
	    const Geom::Line &labelPos,
	    const Styles::OrientedLabel &labelStyle,
	    double centered) const;

	void render(const OrientedLabel &label,
	    const Gfx::Color &textColor,
	    const Gfx::Color &bgColor,
	    const Util::EventDispatcher::event_ptr &event,
	    std::unique_ptr<Util::EventTarget> eventTarget);
};

}

#endif
