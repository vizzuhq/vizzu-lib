#ifndef ITEM_DRAWITEM_H
#define ITEM_DRAWITEM_H

#include <array>
#include <memory>

#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "chart/generator/diagram.h"
#include "chart/generator/marker.h"
#include "chart/main/style.h"
#include "chart/rendering/painter/coordinatesystem.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem
{
public:
	static std::unique_ptr<DrawItem> create(const Diag::Marker &marker,
	    const Diag::Options &options,
	    const Styles::Chart &style,
		const CoordinateSystem &coordSys,
	    const Diag::Diagram::Markers &markers);

	const Diag::Marker &marker;
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

	DrawItem(const Diag::Marker &marker) : marker(marker) {}
	virtual ~DrawItem() {}

	virtual bool bounds(const Geom::Point &) { return false; }
	Geom::Rect getBoundary() const;
	Geom::Line getLine() const;
	Geom::Line getStick() const;
	Geom::Line getLabelPos(Styles::MarkerLabel::Position position,
		const CoordinateSystem &coordSys) const;
};

class SingleDrawItem : public DrawItem
{
public:
	SingleDrawItem(const Diag::Marker &marker,
			 const Diag::Options &options,
			 Diag::ShapeType::Type type);
};

}
}

#endif
