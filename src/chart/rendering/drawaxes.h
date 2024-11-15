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
	[[nodiscard]] Geom::Line getAxis(Gen::AxisId axisIndex) const;
	[[nodiscard]] Geom::Point getTitleBasePos(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index) const;
	[[nodiscard]] Geom::Point getTitleOffset(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index,
	    bool fades) const;
	void drawAxis(Gen::AxisId axisIndex) const;
	void drawTitle(Gen::AxisId axisIndex) const;
	void drawDimensionLabels(bool horizontal) const;
	void drawDimensionLabel(bool horizontal,
	    const Geom::Point &origo,
	    const Gen::DimensionAxis::Item &item,
	    const Data::SliceIndex &index) const;
};

}

#endif
