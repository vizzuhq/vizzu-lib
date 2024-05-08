#ifndef MARKERS_ABSTRACTMARKER_H
#define MARKERS_ABSTRACTMARKER_H

#include <array>

#include "base/geom/line.h"
#include "base/geom/quadrilateral.h"
#include "base/geom/rect.h"
#include "chart/generator/marker.h"
#include "chart/main/style.h"
#include "chart/rendering/drawingcontext.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu::Draw
{

class AbstractMarker
{
public:
	static AbstractMarker createInterpolated(
	    const DrawingContext &ctx,
	    const Gen::Marker &marker,
	    bool lineIndex);

	const Gen::Marker &marker;
	::Anim::Interpolated<Gen::ShapeType> shapeType;
	Math::FuzzyBool enabled;
	Math::FuzzyBool labelEnabled;
	Math::FuzzyBool connected;
	Math::FuzzyBool morphToCircle;
	Math::FuzzyBool linear;
	Math::FuzzyBool border;
	std::array<Geom::Point, 4> points;
	std::array<double, 2> lineWidth{};
	Geom::Point center;
	Geom::Rect dataRect;
	double radius{};

	[[nodiscard]] Geom::Rect getBoundary() const;
	[[nodiscard]] Geom::Line getLine() const;
	[[nodiscard]] Geom::Line getStick() const;
	[[nodiscard]] Geom::Line getLabelPos(
	    Styles::MarkerLabel::Position position,
	    const CoordinateSystem &coordSys) const;

protected:
	AbstractMarker(const Gen::Marker &marker,
	    const Gen::Options &options);

	static AbstractMarker create(const DrawingContext &ctx,
	    const Gen::Marker &marker,
	    const Gen::ShapeType &shapeType,
	    bool lineIndex);
};

class SingleDrawMarker : public AbstractMarker
{
public:
	SingleDrawMarker(const Gen::Marker &marker,
	    const Gen::Options &options,
	    Gen::ShapeType type);
};

}

#endif
