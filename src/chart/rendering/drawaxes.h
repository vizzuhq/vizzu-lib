#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "base/geom/line.h"
#include "chart/generator/axis.h"

#include "drawingcontext.h"
#include "drawinterlacing.h"

namespace Vizzu::Draw
{

class DrawAxes : public DrawingContext
{
public:
	void drawGeometries() const;
	void drawLabels() const;

	Gfx::ICanvas &canvas;
	Painter &painter;
	DrawInterlacing interlacing;

private:
	[[nodiscard]] Geom::Line getAxis(Gen::ChannelId axisIndex) const;
	[[nodiscard]] Geom::Point
	getTitleBasePos(Gen::ChannelId axisIndex, bool index) const;
	[[nodiscard]] Geom::Point getTitleOffset(Gen::ChannelId axisIndex,
	    bool index,
	    bool fades) const;
	void drawAxis(Gen::ChannelId axisIndex) const;
	void drawTitle(Gen::ChannelId axisIndex) const;
	void drawDimensionLabels(bool horizontal) const;
	void drawDimensionLabel(bool horizontal,
	    const Geom::Point &origo,
	    Gen::DimensionAxis::Values::const_iterator it,
	    const std::string_view &category) const;
};

}

#endif
