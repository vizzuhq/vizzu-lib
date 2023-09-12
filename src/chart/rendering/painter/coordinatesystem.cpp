#include "coordinatesystem.h"

#include <cmath>

#include "base/math/interpolation.h"

using namespace Vizzu;
using namespace Vizzu::Draw;
using namespace Geom;

PolarDescartesTransform::PolarDescartesTransform(
    const Anim::Interpolated<Gen::CoordSystem> &coordSystem) :
    zoomOut{true},
    polar(coordSystem.factor<double>(Gen::CoordSystem::polar))
{}

Point PolarDescartesTransform::convert(const Point &p) const
{
	if (polar == false) return p;

	auto mapped = mappedSize();
	auto usedAngle = Math::interpolate(0.0,
	    2.0 * M_PI,
	    static_cast<double>(polar));
	auto hEquidist = mapped.area() / M_PI;
	auto yCircTop = 1.0 - mapped.y;
	auto radius = mapped.x / usedAngle - hEquidist;
	const Point center(0.5, yCircTop - radius);

	auto angle = M_PI / 2.0 + (0.5 - p.x) * usedAngle;

	auto converted =
	    center + Point::Polar(radius + p.y * mapped.y, angle);

	if (zoomOut) {
		auto zoomFactor = static_cast<double>(polar) - 0.5;
		zoomFactor = 0.75 + zoomFactor * zoomFactor;
		return Point{.5, .5}
		     + (converted - Point{.5, .5}) * zoomFactor;
	}
	return converted;
}

double PolarDescartesTransform::horConvert(double length) const
{
	return mappedSize().x * length;
}

double PolarDescartesTransform::verConvert(double length) const
{
	return mappedSize().y * length;
}

Point PolarDescartesTransform::getOriginal(const Point &p) const
{
	if (polar == false) return p;
	if (polar == true) {
		const Point center(0.5, 0.5);
		auto polar = (p - center).toPolar();

		polar.y = -polar.y + 3 * M_PI / 2;
		if (polar.y < 0) polar.y += 2 * M_PI;
		if (polar.y > 2 * M_PI) polar.y -= 2 * M_PI;

		return {polar.y / (2 * M_PI), 2 * polar.x};
	}

	Point pZoomed = p;
	if (zoomOut) {
		auto zoomFactor = static_cast<double>(polar) - 0.5;
		zoomFactor = 0.75 + zoomFactor * zoomFactor;
		pZoomed = Point{.5, .5} + (p - Point{.5, .5}) / zoomFactor;
	}
	auto mapped = mappedSize();
	auto usedAngle = Math::interpolate(0.0,
	    2.0 * M_PI,
	    static_cast<double>(polar));
	auto hEquidist = mapped.area() / M_PI;
	auto yCircTop = 1.0 - mapped.y;
	auto radius = mapped.x / usedAngle - hEquidist;
	const Point center(0.5, yCircTop - radius);

	auto polar = (pZoomed - center).toPolar();
	polar.y = polar.y - M_PI / 2.0;
	while (polar.y < M_PI) polar.y += 2 * M_PI;
	while (polar.y > M_PI) polar.y -= 2 * M_PI;
	return {0.5 - polar.y / usedAngle, (polar.x - radius) / mapped.y};
}

Math::FuzzyBool PolarDescartesTransform::getPolar() const
{
	return polar;
}

bool PolarDescartesTransform::atEndState() const
{
	return polar == false || polar == true;
}

Size PolarDescartesTransform::mappedSize() const
{
	return {1.0, (2.0 - static_cast<double>(polar)) / 2.0};
}

CompoundTransform::CompoundTransform(const Rect &rect,
    double angle,
    const Anim::Interpolated<Gen::CoordSystem> &coordSystem,
    Math::FuzzyBool keepAspectRatio) :
    PolarDescartesTransform(coordSystem),
    rect(rect),
    keepAspectRatio(keepAspectRatio)
{
	setAngle(angle);
}

void CompoundTransform::setAngle(double value)
{
	angle = value - static_cast<double>(polar) * M_PI;
	cosAngle = cos(angle);
	sinAngle = sin(angle);
}

double CompoundTransform::getAngle() const
{
	return angle + static_cast<double>(polar) * M_PI;
}

Point CompoundTransform::convert(const Point &p) const
{
	auto transformed = PolarDescartesTransform::convert(p);
	auto rotated = rotate(transformed);
	auto aligned = align(rotated);
	return rect.pos + rect.size * Point{aligned.x, 1 - aligned.y};
}

Line CompoundTransform::convertDirectionAt(const Line &vec) const
{
	const auto small = .00000000001;

	auto base = vec.begin;
	auto dir = vec.getDirection();
	auto maxDir = dir.chebyshev();
	auto smallDir = (maxDir != 0.0) ? dir / maxDir * small
	                                : Geom::Point{0.0, small};
	auto end = base + smallDir;

	auto baseConverted = convert(base);
	auto endConverted = convert(end);

	auto dirConverted = (endConverted - baseConverted).normalized();

	endConverted = baseConverted + dirConverted;

	return {baseConverted, endConverted};
}

double CompoundTransform::horConvert(double length) const
{
	return (rotatedSize() * alignedSize()).x
	     * PolarDescartesTransform::horConvert(length);
}

double CompoundTransform::verConvert(double length) const
{
	return (rotatedSize() * alignedSize()).y
	     * PolarDescartesTransform::verConvert(length);
}

Point CompoundTransform::getOriginal(const Point &p) const
{
	auto relative = (p - rect.pos) / rect.size;
	relative = Point{relative.x, 1.0 - relative.y};
	relative = deAlign(relative);
	auto rotated = rotate(relative, true);
	return PolarDescartesTransform::getOriginal(rotated);
}

Rect CompoundTransform::getRect() const { return rect; }

Point CompoundTransform::justRotate(const Point &p) const
{
	return rotate(p, true, Geom::Point());
}

Point CompoundTransform::rotate(const Point &point,
    bool invert,
    const Point &center) const
{
	auto centered = point - center;

	const Geom::Point rotated(cosAngle * centered.x
	                              + (invert ? 1 : -1) * sinAngle
	                                    * centered.y,
	    (invert ? -1 : 1) * sinAngle * centered.x
	        + cosAngle * centered.y);

	return center + rotated;
}

Size CompoundTransform::rotatedSize() const
{
	auto cosAng = cos(getAngle());
	auto sinAng = sin(getAngle());

	auto cosAbs = cosAng >= 0 ? cosAng : -cosAng;
	auto sinAbs = sinAng >= 0 ? sinAng : -sinAng;

	auto x = cosAbs * rect.size.x + sinAbs * rect.size.y;
	auto y = cosAbs * rect.size.y + sinAbs * rect.size.x;

	return {x, y};
}

Geom::Size CompoundTransform::alignedSize() const
{
	auto minAspectRatio = std::min(rect.size.aspectRatio(), 1.0);

	auto factor = static_cast<double>((polar) || keepAspectRatio);

	auto aspectRatioVer =
	    Math::interpolate(1.0, minAspectRatio, factor);

	auto aspectRatioHor = Math::interpolate(1.0,
	    minAspectRatio / rect.size.aspectRatio(),
	    factor);

	return {aspectRatioHor, aspectRatioVer};
}

Point CompoundTransform::align(const Point &point) const
{
	const Point half(0.5, 0.5);
	return half + alignedSize() * (point - half);
}

Point CompoundTransform::deAlign(const Point &point) const
{
	const Point half(0.5, 0.5);
	return half + (point - half) / alignedSize();
}
