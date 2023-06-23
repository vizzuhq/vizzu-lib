#ifndef OUTPUT_CANVAS_JSCRIPT_H
#define OUTPUT_CANVAS_JSCRIPT_H

#include <functional>
#include <optional>

#include "base/gfx/canvas.h"
#include "chart/rendering/painter/painter.h"

namespace Vizzu::Main
{

class JScriptCanvas : public Gfx::ICanvas,
                      public Draw::Painter
{
public:
	JScriptCanvas() = default;
	~JScriptCanvas() = default;

	Geom::Size textBoundary(const std::string &text) override;

	Geom::Rect getClipRect() const override;
	void setClipRect(const Geom::Rect &rect) override;
	void setClipCircle(const Geom::Circle &circle) override;
	void setClipPolygon() override;
	void setBrushColor(const Gfx::Color &color) override;
	void setLineColor(const Gfx::Color &color) override;
	void setLineWidth(double width) override;
	void setFont(const Gfx::Font &font) override;
	void setTextColor(const Gfx::Color &color) override;

	void beginDropShadow() override;
	void setDropShadowBlur(uint64_t radius) override;
	void setDropShadowColor(const Gfx::Color &color) override;
	void setDropShadowOffset(const Geom::Point &offset) override;
	void endDropShadow() override;

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

	void frameBegin() override;
	void frameEnd() override;

	void transform(const Geom::AffineTransform &transform) override;
	void save() override;
	void restore() override;

	Gfx::ICanvas &getCanvas() override { return *this; }

	void *getPainter() override {
		return static_cast<Draw::Painter*>(this);
	}
private:
	void resetStates();
	std::string domId;
	std::optional<Gfx::Font> font;
	std::optional<Gfx::Color> brushColor;
	std::optional<Gfx::Color> lineColor;
	std::optional<double> lineWidth;
	std::optional<Geom::Rect> clipRect;
};

}

#endif