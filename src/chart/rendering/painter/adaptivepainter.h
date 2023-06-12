#ifndef ADAPTIVEPAINTER_H
#define ADAPTIVEPAINTER_H

#include "painter.h"

namespace Vizzu
{
namespace Draw
{

class AdaptivePainter : public IPainter
{
public:
	using IPainter::IPainter;

	void copyOptionsTo(IPainter &painter) const;

	void setCoordSys(const CoordinateSystem &system) override
	{
		this->system = system;
	}
	void setResMode(const ResolutionMode &mode) override
	{
		this->mode = mode;
	}

	void drawLine(const Geom::Line &line) override;

	void drawStraightLine(const Geom::Line &line,
	    std::array<double, 2> widths,
	    const Gfx::Color &endColor,
	    const Gfx::Color &lineColor) override;

	void setPolygonToCircleFactor(double factor) override
	{
		polygonOptions.toCircleFactor = factor;
	}
	void setPolygonStraightFactor(double factor) override
	{
		polygonOptions.straightFactor = factor;
	}

	void drawPolygon(const std::array<Geom::Point, 4> &ps,
	    bool clip) override;

private:
	struct PolygonOptions
	{
		double toCircleFactor;
		double straightFactor;
	};

	CoordinateSystem system;
	ResolutionMode mode;
	PolygonOptions polygonOptions;
};

}
}

#endif
