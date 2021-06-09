#ifndef ITEM_DRAWITEM_H
#define ITEM_DRAWITEM_H

#include <array>
#include <memory>

#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "chart/generator/diagram.h"
#include "chart/generator/marker.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem
{
public:
	static std::unique_ptr<DrawItem> create(const Diag::Marker &item,
	    const Diag::DiagramOptions &options,
	    const Styles::Chart &style,
	    const Diag::Diagram::Markers &items);

	Math::FuzzyBool enabled;
	double connected;
	Math::FuzzyBool morphToCircle;
	Math::FuzzyBool linear;
	Math::FuzzyBool border;
	Geom::Size space;
	std::array<Geom::Point, 4> points;
	std::array<double, 2> lineWidth;
	Gfx::Color color;
	Geom::Point center;

	virtual ~DrawItem() {}

	virtual bool bounds(const Geom::Point &) { return false; }
	Geom::Rect getBoundary() const;
	Geom::Line getLine() const;
	Geom::Line getStick() const;
};

class SingleDrawItem : public DrawItem
{
public:
	SingleDrawItem(const Diag::Marker &item,
			 const Diag::DiagramOptions &options,
			 Diag::ShapeType::Type type);
};

}
}

#endif
