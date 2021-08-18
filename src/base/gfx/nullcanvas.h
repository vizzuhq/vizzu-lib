#ifndef GFX_NULLCANVAS
#define GFX_NULLCANVAS

#include "canvas.h"

namespace Gfx
{

struct NullCanvas : public ICanvas
{
	ICanvasPtr createCanvas(int, int) override;
	Geom::Size textBoundary(const std::string &) override
	{
		return Geom::Size();
	}

	Geom::Rect getClipRect() const override { return Geom::Rect(); }
	void setClipRect(const Geom::Rect &, bool) override {}
	void setClipPolygon(bool) override {}
	void setBrushColor(const Color &) override {}
	void setLineColor(const Color &) override {}
	void setTextColor(const Color &) override {}
	void setLineWidth(double) override {}
	void setFont(const Gfx::Font &) override {}

	void beginDropShadow() override {}
	void setDropShadowBlur(uint64_t) override {}
	void setDropShadowColor(const Gfx::Color &) override {}
	void setDropShadowOffset(const Geom::Point &) override {}
	void endDropShadow() override {}

	void beginPolygon() override {}
	void addPoint(const Geom::Point &) override {}
	void addBezier(const Geom::Point &,
	    const Geom::Point &,
	    const Geom::Point &) override {}
	void endPolygon() override {}
	void rectangle(const Geom::Rect &) override {}
	void circle(const Geom::Circle &) override {}
	void line(const Geom::Line &) override {}
	void text(const Geom::Rect &, const std::string &) override {}
	void setBrushGradient(const Geom::Line &, const ColorGradient &) override {}
	int loadSvgImage(const Gfx::Svg &) override { return 0; }
	int loadPixMapImage(const Gfx::PixMapView &) override { return 0; }
	void drawImage(int, const Geom::Rect &, double) override {}
	void dropImage(int) override {}
	void drawCanvas(const Geom::Rect &, const ICanvas &) override {}

	void frameBegin() override {}
	void frameEnd() override {}

	void pushTransform(const Geom::AffineTransform&) override {}
	void popTransform() override {}
};

}

#endif
