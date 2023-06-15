#ifndef QT_CANVAS_H
#define QT_CANVAS_H

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPicture>

#include "base/gfx/canvas.h"
#include "chart/rendering/painter/adaptivepainter.h"

class BaseCanvas :
    public Gfx::ICanvas,
    public Vizzu::Draw::AdaptivePainter
{
public:
	BaseCanvas(QPaintDevice *device = nullptr);
	~BaseCanvas() override;
	void init(QPaintDevice *device);

	Geom::Size textBoundary(const std::string &text) override;

	Gfx::ICanvas &getCanvas() override { return *this; }

	Geom::Rect getClipRect() const override;
	void setClipRect(const Geom::Rect &rect) override;
	void setClipCircle(const Geom::Circle &circle) override;
	void setClipPolygon() override;
	void setBrushColor(const Gfx::Color &color) override;
	void setLineColor(const Gfx::Color &color) override;
	void setLineWidth(double width) override;
	void setFont(const Gfx::Font &newFont) override;
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

	void frameBegin() override {}
	void frameEnd() override {}

	void transform(const Geom::AffineTransform &transform) override;
	void save() override;
	void restore() override;

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

typedef BaseCanvas Canvas;

class PictureCanvas : public Canvas
{
public:
	PictureCanvas() { init(&picture); }
	~PictureCanvas()
	{
		if (painter.isActive()) painter.end();
	}

private:
	QPicture picture;
};

class ImageCanvas : public Canvas
{
public:
	ImageCanvas(const QString &filename, int width, int height) :
	    image(width, height, QImage::Format_RGB32),
	    filename(filename)
	{
		image.fill(Qt::white);
		init(&image);
	}
	~ImageCanvas()
	{
		if (painter.isActive()) painter.end();
		if (image.save(filename))
			qInfo() << filename << "saved";
		else
			qWarning() << filename << "saving failed";
	}

private:
	QImage image;
	QString filename;
};

#endif
