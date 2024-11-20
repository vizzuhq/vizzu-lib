#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "base/geom/line.h"
#include "chart/generator/axis.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawAxes : public DrawingContext
{
public:
	void drawGeometries() const;
	void drawLabels() const;

	Gfx::ICanvas &canvas;
	Painter &painter;

	const Gen::Axis &getAxis(Gen::AxisId axisIndex) const
	{
		return plot->axises.at(axisIndex);
	}

	Geom::Point origo() const { return plot->axises.origo(); }

private:
	[[nodiscard]] Geom::Line getAxisLine(Gen::AxisId axisIndex) const;
	[[nodiscard]] Geom::Point getTitleBasePos(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index) const;
	[[nodiscard]] Geom::Point getTitleOffset(Gen::AxisId axisIndex,
	    ::Anim::InterpolateIndex index,
	    bool fades) const;
	void drawAxis(Gen::AxisId axisIndex) const;
	void drawTitle(Gen::AxisId axisIndex) const;
	void drawDimensionLabels(Gen::AxisId axisIndex) const;
	void drawDimensionLabel(Gen::AxisId axisIndex,
	    const Geom::Point &origo,
	    const Gen::DimensionAxis::Item &item,
	    const Data::SliceIndex &index,
	    double weight) const;
};

}

#endif
