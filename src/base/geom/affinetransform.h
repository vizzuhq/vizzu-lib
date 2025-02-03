#ifndef GEOM_AFFINETRANSFORM
#define GEOM_AFFINETRANSFORM

#include <array>

#include "point.h"

namespace Geom
{

struct Size;
struct Rect;
class Circle;
struct Line;
class Polygon;

class AffineTransform
{
public:
	using Row = std::array<double, 3>;
	using Matrix = std::array<Row, 2>;

	AffineTransform();
	explicit AffineTransform(const Matrix &m) : m(m){};
	AffineTransform(double m00,
	    double m01,
	    double m02,
	    double m10,
	    double m11,
	    double m12);
	explicit AffineTransform(Point offset,
	    double scale = 1.0,
	    double angle = 0.0);

	[[nodiscard]] const Matrix &getMatrix() const { return m; }

	[[nodiscard]] AffineTransform inverse() const;
	[[nodiscard]] bool transforms() const;
	void shift(const Point &offset);

	friend AffineTransform operator*(AffineTransform lhs,
	    const AffineTransform &rhs)
	{
		return lhs *= rhs;
	}
	AffineTransform &operator*=(const AffineTransform &other);
	bool operator==(const AffineTransform &other) const = default;

	Point operator()(const Point &original) const;
	Line operator()(const Line &original) const;
	Polygon operator()(const Polygon &original) const;
	Size operator()(const Size &original) const;

	[[nodiscard]] std::string toJSON() const;

private:
	Matrix m;
};

}

#endif
