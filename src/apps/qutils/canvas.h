#ifndef QT_CANVAS_H
#define QT_CANVAS_H

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPicture>

#include "base/gfx/canvas.h"
#include "chart/rendering/painter/painter.h"

class BaseCanvas : public Gfx::ICanvas, public Vizzu::Draw::Painter
{
public:
	explicit BaseCanvas(QPaintDevice *device = nullptr);
	~BaseCanvas() override;
	void init(QPaintDevice *device);

	Gfx::ICanvas &getCanvas() override { return *this; }

	void setClipRect(const Geom::Rect &rect) override;
	void setClipCircle(const Geom::Circle &circle) override;
	void setClipPolygon() override;
	void setBrushColor(const Gfx::Color &color) override;
	void setLineColor(const Gfx::Color &color) override;
	void setLineWidth(double width) override;
	void setFont(const Gfx::Font &newFont) override;
	void setTextColor(const Gfx::Color &color) override;

	void beginDropShadow() override;
	void setDropShadowBlur(double radius) override;
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

	void text(const Geom::Rect &rect, std::string_view text) override;

	void setBrushGradient(const Geom::Line &line,
	    const Gfx::ColorGradient &gradient) override;

	void frameBegin() override {}
	void frameEnd() override {}

	void transform(const Geom::AffineTransform &transform) override;
	void save() override;
	void restore() override;

	void *getPainter() override
	{
		return static_cast<Vizzu::Draw::Painter *>(this);
	}

	[[nodiscard]] static QFont fromGfxFont(const Gfx::Font &newFont,
	    QFont font = {});

protected:
	QPainter painter;
	QFont font;
	QPainterPath polygon;
	QPen linePen;
	QPen textPen;
	QBrush brush;

	QPen colorToPen(const Gfx::Color &color);
	QPen brushToPen(const QBrush &brush);
};

using Canvas = BaseCanvas;

#endif
