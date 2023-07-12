#ifndef ITEM_DRAWITEM_H
#define ITEM_DRAWITEM_H

#include <array>
#include <memory>

#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "base/geom/quadrilateral.h"
#include "chart/generator/marker.h"
#include "chart/generator/plot.h"
#include "chart/main/style.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem
{
public:

	static DrawItem createInterpolated(
	    const Gen::Marker &marker,
	    const Gen::Options &options,
	    const Styles::Chart &style,
	    const CoordinateSystem &coordSys,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);

	const Gen::Marker &marker;
	const CoordinateSystem &coordSys;
	::Anim::Interpolated<Gen::ShapeType> shapeType;
	Math::FuzzyBool enabled;
	Math::FuzzyBool labelEnabled;
	Math::FuzzyBool connected;
	Math::FuzzyBool morphToCircle;
	Math::FuzzyBool linear;
	Math::FuzzyBool border;
	std::array<Geom::Point, 4> points;
	std::array<double, 2> lineWidth;
	Gfx::Color color;
	Geom::Point center;
	Geom::Rect dataRect;
	double radius;

	bool bounds(const Geom::Point &);
	Geom::Rect getBoundary() const;
	Geom::Line getLine() const;
	Geom::Line getStick() const;
	Geom::Line getLabelPos(Styles::MarkerLabel::Position position,
	    const CoordinateSystem &coordSys) const;

protected:

	DrawItem(const Gen::Marker &marker,
		const CoordinateSystem &coordSys,
		const Gen::Options &options);

	static DrawItem create(
	    const Gen::Marker &marker,
	    const Gen::Options &options,
	    const Gen::ShapeType &shapeType,
	    const Styles::Chart &style,
	    const CoordinateSystem &coordSys,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);

private:
	Geom::ConvexQuad lineToQuad(double atLeastWidth = 0.0) const;
};

class SingleDrawItem : public DrawItem
{
public:
	SingleDrawItem(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    Gen::ShapeType type);
};

}
}

#endif
