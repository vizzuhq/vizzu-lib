#ifndef MARKERS_ABSTRACTMARKER_H
#define MARKERS_ABSTRACTMARKER_H

#include <array>
#include <memory>

#include "base/geom/line.h"
#include "base/geom/quadrilateral.h"
#include "base/geom/rect.h"
#include "chart/generator/marker.h"
#include "chart/generator/plot.h"
#include "chart/main/style.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu::Draw
{

class AbstractMarker
{
public:
	static AbstractMarker createInterpolated(
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
	std::array<double, 2> lineWidth{};
	Gfx::Color color;
	Geom::Point center;
	Geom::Rect dataRect;
	double radius{};

	bool bounds(const Geom::Point &);
	[[nodiscard]] Geom::Rect getBoundary() const;
	[[nodiscard]] Geom::Line getLine() const;
	[[nodiscard]] Geom::Line getStick() const;
	[[nodiscard]] Geom::Line getLabelPos(
	    Styles::MarkerLabel::Position position,
	    const CoordinateSystem &coordSys) const;

protected:
	AbstractMarker(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options);

	static AbstractMarker create(const Gen::Marker &marker,
	    const Gen::Options &options,
	    const Gen::ShapeType &shapeType,
	    const Styles::Chart &style,
	    const CoordinateSystem &coordSys,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);

private:
	[[nodiscard]] Geom::ConvexQuad lineToQuad(
	    double atLeastWidth = 0.0) const;
};

class SingleDrawMarker : public AbstractMarker
{
public:
	SingleDrawMarker(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    Gen::ShapeType type);
};

}

#endif
