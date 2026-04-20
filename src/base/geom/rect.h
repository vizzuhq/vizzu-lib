#ifndef GEOM_RECT
#define GEOM_RECT

#include <array>
#include <functional>
#include <ranges>
#include <tuple>
#include <vector>

#include "base/math/range.h"

#include "line.h"
#include "point.h"

namespace Geom
{

struct Rect
{
	Point pos{};
	Size size{};

	template <class Container>
	static Rect Boundary(const Container &points);

	static Rect Ident();

	[[nodiscard]] Rect boundary(const Rect &rect) const;
	[[nodiscard]] Point normalize(const Point &p) const;
	[[nodiscard]] Size normalize(const Size &s) const;
	[[nodiscard]] Rect normalize(const Rect &rect) const;
	[[nodiscard]] Rect positive() const;

	bool operator==(const Rect &other) const
	{
		return pos == other.pos && size == other.size;
	}

	Rect operator*(double factor) const;
	Rect operator+(const Geom::Rect &other) const;

	[[nodiscard]] double left() const { return pos.x; }
	[[nodiscard]] double right() const { return pos.x + size.x; }
	[[nodiscard]] double top() const { return pos.y + size.y; }
	[[nodiscard]] double bottom() const { return pos.y; }

	[[nodiscard]] double width() const { return size.x; }
	[[nodiscard]] double height() const { return size.y; }

	[[nodiscard]] Math::Range<> hSize() const
	{
		return {left(), right()};
	}
	[[nodiscard]] Math::Range<> vSize() const
	{
		return {bottom(), top()};
	}

	[[nodiscard]] Math::Range<> oSize(Orientation o) const
	{
		return isHorizontal(o) ? hSize() : vSize();
	}

	[[nodiscard]] Math::Range<> x() const
	{
		return {pos.x, pos.x + size.x};
	}
	[[nodiscard]] Math::Range<> y() const
	{
		return {pos.y, pos.y + size.y};
	}

	[[nodiscard]] Point topLeft() const { return {left(), top()}; }
	[[nodiscard]] Point topRight() const { return {right(), top()}; }
	[[nodiscard]] Point bottomLeft() const { return pos; }
	[[nodiscard]] Point bottomRight() const
	{
		return {right(), bottom()};
	}

	void setLeft(double val)
	{
		size.x -= val - pos.x;
		pos.x = val;
	}
	void setRight(double val) { size.x = val - pos.x; }
	void setTop(double val) { size.y = val - pos.y; }
	void setBottom(double val)
	{
		size.y -= val - pos.y;
		pos.y = val;
	}

	void setWidth(double val) { size.x = val; }
	void setHeight(double val) { size.y = val; }

	void setHSize(const Math::Range<> &range)
	{
		setLeft(range.min);
		setRight(range.max);
	}

	void setVSize(const Math::Range<> &range)
	{
		setBottom(range.min);
		setTop(range.max);
	}

	void setOSize(Orientation o, const Math::Range<> &range)
	{
		if (isHorizontal(o))
			setHSize(range);
		else
			setVSize(range);
	}

	[[nodiscard]] Rect bottomHalf() const
	{
		return {pos, size.verticalHalf()};
	}
	[[nodiscard]] Rect topHalf() const
	{
		return {{left(), center().y}, size.verticalHalf()};
	}
	[[nodiscard]] Rect leftHalf() const
	{
		return {pos, size.horizontalHalf()};
	}
	[[nodiscard]] Rect rightHalf() const
	{
		return {{center().x, bottom()}, size.horizontalHalf()};
	}

	[[nodiscard]] Line leftSide() const
	{
		return {topLeft(), bottomLeft()};
	}
	[[nodiscard]] Line rightSide() const
	{
		return {topRight(), bottomRight()};
	}
	[[nodiscard]] Line topSide() const
	{
		return {topLeft(), topRight()};
	}
	[[nodiscard]] Line bottomSide() const
	{
		return {bottomLeft(), bottomRight()};
	}

	[[nodiscard]] Rect intersection(const Rect &rect) const;

	[[nodiscard]] bool contains(const Point &p) const;
	[[nodiscard]] Point center() const;

	[[nodiscard]] Rect outline(const Geom::Size &margin) const
	{
		return {pos - margin, {size + margin * 2}};
	}

	Rect popBottom(double length);
	Rect popTop(double length);
	Rect popLeft(double length);
	Rect popRight(double length);

	[[nodiscard]] std::array<Point, 4> points() const;
};

template <class Container>
Rect Rect::Boundary(const Container &points)
{
	auto &&[minx, maxx] = std::ranges::minmax(
	    std::views::transform(points, std::mem_fn(&Point::x)),
	    Math::Floating::less);
	auto &&[miny, maxy] = std::ranges::minmax(
	    std::views::transform(points, std::mem_fn(&Point::y)),
	    Math::Floating::less);
	return Rect{{minx, miny}, {maxx - minx, maxy - miny}};
}

}

#endif
