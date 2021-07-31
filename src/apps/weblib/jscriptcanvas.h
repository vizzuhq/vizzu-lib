#ifndef OUTPUT_CANVAS_JSCRIPT_H
#define OUTPUT_CANVAS_JSCRIPT_H

#include <functional>
#include <optional>

#include "base/gfx/canvas.h"
#include "chart/rendering/painter/adaptivepainter.h"

namespace Vizzu
{
namespace Main
{

class JScriptOutputCanvas : public Gfx::ICanvas
{
public:
	JScriptOutputCanvas();
	~JScriptOutputCanvas();

	Gfx::ICanvasPtr createCanvas(int width, int height) override;

	Geom::Size textBoundary(const std::string &text) override;

	Geom::Rect getClipRect() const override;
	void setClipRect(const Geom::Rect &rect, bool clear) override;
	void setClipPolygon(bool clear) override;
	void setBrushColor(const Gfx::Color &color) override;
	void setLineColor(const Gfx::Color &color) override;
	void setLineWidth(double width) override;
	void setFont(const Gfx::Font &font) override;
	void setTextColor(const Gfx::Color &color) override;

	void beginPolygon() override;
	void addPoint(const Geom::Point &point) override;
	void addBezier(const Geom::Point &control0,
	    const Geom::Point &control1,
	    const Geom::Point &endPoint) override;
	void endPolygon() override;

	void rectangle(const Geom::Rect &rect) override;
	void circle(const Geom::Circle &circle) override;
	void line(const Geom::Line &line) override;

	void text(const Geom::Rect &rect,
			  const std::string &text) override;

	void setBrushGradient(const Geom::Line &line,
						  const Gfx::ColorGradient &gradient) override;

	int loadSvgImage(const Gfx::Svg &svg) override;
	int loadPixMapImage(const Gfx::PixMapView &pixmap) override;
	void drawImage(int imageId,
				   const Geom::Rect &boundary,
				   double opacity) override;
	void dropImage(int imageId) override;

	void drawCanvas(const Geom::Rect &rect, const ICanvas &canvas) override;

	void frameBegin() override;
	void frameEnd() override;

	void pushTransform(const Geom::AffineTransform &transform) override;
	void popTransform() override;

private:
	void resetStates();
	std::optional<Gfx::Font> font;
	std::optional<Gfx::Color> brushColor;
	std::optional<Gfx::Color> lineColor;
	std::optional<double> lineWidth;
	Geom::Rect clipRect;
};

template<class Canvas>
class OutputCanvasPainterBase :
	public Canvas,
	public Draw::AdaptivePainter
{
public:
	using Canvas::Canvas;
	Gfx::ICanvas &getCanvas() override { return *this; }
};

typedef OutputCanvasPainterBase<JScriptOutputCanvas> JScriptCanvas;

}
}

#endif