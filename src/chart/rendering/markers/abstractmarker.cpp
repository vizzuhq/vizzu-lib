#include "abstractmarker.h"

#include <array>
#include <cstddef>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/line.h"
#include "base/math/floating.h"
#include "chart/generator/marker.h"
#include "chart/main/style.h"
#include "chart/options/shapetype.h"
#include "chart/rendering/drawingcontext.h"
#include "chart/rendering/painter/coordinatesystem.h"

#include "circlemarker.h"
#include "connectingmarker.h"
#include "rectanglemarker.h"

namespace Vizzu::Draw
{

template <class T, std::size_t I>
std::array<T, I> interpolate(const std::array<T, I> &op0,
    const std::array<T, I> &op1,
    double factor)
{
	return
	    []<std::size_t... Ix>(const std::array<T, sizeof...(Ix)> &op0,
	        const std::array<T, sizeof...(Ix)> &op1,
	        double factor,
	        std::index_sequence<Ix...>)
	{
		using Math::interpolate;
		return std::array<T, sizeof...(Ix)>{
		    interpolate(std::get<Ix>(op0),
		        std::get<Ix>(op1),
		        factor)...};
	}(op0, op1, factor, std::make_index_sequence<I>{});
}

AbstractMarker AbstractMarker::create(const DrawingContext &ctx,
    const Gen::Marker &marker,
    const Gen::ShapeType &shapeType,
    ::Anim::InterpolateIndex lineIndex)
{
	switch (shapeType) {
	case Gen::ShapeType::rectangle:
		return RectangleMarker(marker,
		    ctx.coordSys,
		    ctx.getOptions(),
		    ctx.rootStyle);
	case Gen::ShapeType::circle:
		return CircleMarker(marker,
		    ctx.coordSys,
		    ctx.getOptions(),
		    ctx.rootStyle);
	case Gen::ShapeType::area:
	case Gen::ShapeType::line:
		return ConnectingMarker(ctx, marker, lineIndex, shapeType);
	default: return {marker, ctx.getOptions()};
	}
}

AbstractMarker AbstractMarker::createInterpolated(
    const DrawingContext &ctx,
    const Gen::Marker &marker,
    ::Anim::InterpolateIndex lineIndex)
{
	const auto &options = ctx.getOptions();

	auto fromShapeType = options.geometry.values[0].value;

	auto fromMarker = create(ctx, marker, fromShapeType, lineIndex);

	auto toShapeType =
	    options.geometry.get_or_first(::Anim::second).value;

	if (fromShapeType == toShapeType) return fromMarker;

	auto toMarker = create(ctx, marker, toShapeType, lineIndex);

	AbstractMarker aMarker(marker, options);
	aMarker.enabled = fromMarker.enabled + toMarker.enabled;
	aMarker.labelEnabled =
	    fromMarker.labelEnabled + toMarker.labelEnabled;

	auto enableFactorBase = static_cast<double>(toMarker.enabled);
	auto labelEnableFactorBase =
	    static_cast<double>(toMarker.labelEnabled);
	auto enableSum = static_cast<double>(aMarker.enabled);
	auto labelEnableSum = static_cast<double>(aMarker.labelEnabled);

	if (marker.polarConnection.interpolates()) {
		auto firstWeight = options.geometry.values[0].weight;
		auto secondWeight = options.geometry.values[1].weight;
		enableSum = firstWeight + secondWeight;
		enableFactorBase = secondWeight;
		labelEnableSum = firstWeight + secondWeight;
		labelEnableFactorBase = secondWeight;
	}

	using Math::interpolate;
	using Vizzu::Draw::interpolate;
	if (!Math::Floating::is_zero(enableSum)) {
		auto factor = enableFactorBase / enableSum;
		aMarker.morphToCircle = interpolate(fromMarker.morphToCircle,
		    toMarker.morphToCircle,
		    factor);
		aMarker.linear =
		    interpolate(fromMarker.linear, toMarker.linear, factor);
		aMarker.border =
		    interpolate(fromMarker.border, toMarker.border, factor);
		aMarker.points =
		    interpolate(fromMarker.points, toMarker.points, factor);
		aMarker.lineWidth = interpolate(fromMarker.lineWidth,
		    toMarker.lineWidth,
		    factor);
		aMarker.connected = interpolate(fromMarker.connected,
		    toMarker.connected,
		    factor);
		aMarker.center =
		    interpolate(fromMarker.center, toMarker.center, factor);
	}
	if (!Math::Floating::is_zero(labelEnableSum)) {
		auto factor = labelEnableFactorBase / labelEnableSum;
		aMarker.dataRect = interpolate(fromMarker.dataRect,
		    toMarker.dataRect,
		    factor);
		aMarker.radius =
		    interpolate(fromMarker.radius, toMarker.radius, factor);
	}
	return aMarker;
}

void AbstractMarker::setDataPosition(const CoordinateSystem &coordSys)
{
	dataPosition = {
	    this->getLabelPos(Styles::MarkerLabel::Position::top,
	            coordSys)
	        .end,
	    this->getLabelPos(Styles::MarkerLabel::Position::center,
	            coordSys)
	        .begin};
}

Geom::Rect AbstractMarker::getBoundary() const
{
	return Geom::Rect::Boundary(points);
}

Geom::Line AbstractMarker::getLine() const
{
	return {points[3], points[2]};
}

Geom::Line AbstractMarker::getStick() const
{
	return {points[1], points[2]};
}

Geom::Line AbstractMarker::getLabelPos(
    Styles::MarkerLabel::Position position,
    const CoordinateSystem &coordSys) const
{
	Geom::Point center;
	Geom::Point direction;

	typedef Styles::MarkerLabel::Position Pos;
	switch (position) {
	default:
	case Pos::center:
	case Pos::top: direction = Geom::Point{0, 1}; break;
	case Pos::bottom: direction = Geom::Point{0, -1}; break;
	case Pos::left: direction = Geom::Point{-1, 0}; break;
	case Pos::right: direction = Geom::Point{1, 0}; break;
	}

	if (position == Pos::center) { center = dataRect.center(); }
	else {
		Geom::Line side;
		switch (position) {
		default:
		case Pos::top: side = dataRect.positive().topSide(); break;
		case Pos::bottom:
			side = dataRect.positive().bottomSide();
			break;
		case Pos::left: side = dataRect.positive().leftSide(); break;
		case Pos::right:
			side = dataRect.positive().rightSide();
			break;
		}
		center = side.center();
	}

	auto res =
	    coordSys.convertDirectionAt({center, center + direction});

	if (position != Pos::center)
		res.shift(res.getDirection() * radius);

	return res;
}

AbstractMarker::AbstractMarker(const Gen::Marker &marker,
    const Gen::Options &options) :
    marker(marker),
    shapeType(options.geometry),
    enabled(false),
    labelEnabled(false)
{}

SingleDrawMarker::SingleDrawMarker(const Gen::Marker &marker,
    const Gen::Options &options,
    Gen::ShapeType type) :
    AbstractMarker(marker, options)
{
	enabled = options.geometry.factor<Math::FuzzyBool>(type)
	       && marker.enabled;

	labelEnabled = enabled;
}

}