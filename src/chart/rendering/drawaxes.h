#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "base/geom/line.h"

#include "drawingcontext.h"
#include "drawinterlacing.h"

namespace Vizzu::Draw
{

class DrawAxes : public DrawingContext
{
public:
	void drawGeometries();
	void drawLabels();

	Gfx::ICanvas &canvas;
	Painter &painter;
	DrawInterlacing interlacing{{*this}, canvas, painter};

private:
	[[nodiscard]] Geom::Line getAxis(Gen::ChannelId axisIndex) const;
	[[nodiscard]] Geom::Point
	getTitleBasePos(Gen::ChannelId axisIndex, int index) const;
	[[nodiscard]] Geom::Point getTitleOffset(Gen::ChannelId axisIndex,
	    int index,
	    bool fades) const;
	void drawAxis(Gen::ChannelId axisIndex);
	void drawTitle(Gen::ChannelId axisIndex);
	void drawDimensionLabels(bool horizontal);
	void drawDimensionLabel(bool horizontal,
	    const Geom::Point &origo,
	    Gen::DimensionAxis::Values::const_iterator it);
};

}

#endif
