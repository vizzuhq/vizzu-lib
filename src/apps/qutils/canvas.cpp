#include "canvas.h"

#include <QDebug>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QLinearGradient>
#include <QScreen>

QColor toQColor(const Gfx::Color &color)
{
	return {color.getRedByte(),
	    color.getGreenByte(),
	    color.getBlueByte(),
	    color.getAlphaByte()};
}

QPointF toQPoint(const Geom::Point &point)
{
	return {point.x, point.y};
}

QPoint toQPointInt(const Geom::Point &point)
{
	return {static_cast<int>(point.x), static_cast<int>(point.y)};
}

Geom::Point fromQPointF(const QPointF &point)
{
	return {point.x(), point.y()};
}

QLineF toQLine(const Geom::Line &line)
{
	return {toQPoint(line.begin), toQPoint(line.end)};
}

QSizeF toQSize(const Geom::Size &size) { return {size.x, size.y}; }

QSize toQSizeInt(const Geom::Size &size)
{
	return {static_cast<int>(size.x), static_cast<int>(size.y)};
}

Geom::Size fromQSizeF(const QSizeF &size)
{
	return {size.width(), size.height()};
}

QRectF toQRect(const Geom::Rect &rect)
{
	return {toQPoint(rect.pos), toQSize(rect.size)};
}

QRect toQRectInt(const Geom::Rect &rect)
{
	return {toQPointInt(rect.pos), toQSizeInt(rect.size)};
}

Geom::Rect fromQRectF(const QRectF &rect)
{
	return {fromQPointF(rect.topLeft()), fromQSizeF(rect.size())};
}

BaseCanvas::BaseCanvas(QPaintDevice *device)
{
	if (device) init(device);
}

BaseCanvas::~BaseCanvas()
{
	if (painter.isActive()) painter.end();
}

void BaseCanvas::init(QPaintDevice *device)
{
	if (!painter.isActive()) painter.begin(device);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	font = painter.font();
	font.setHintingPreference(QFont::PreferFullHinting);
	font.setStyleStrategy(QFont::PreferAntialias);
	painter.setFont(font);

	auto pen = painter.pen();
	pen.setWidth(0);
	painter.setPen(pen);
}

void BaseCanvas::setBrushColor(const Gfx::Color &color)
{
	brush = QBrush(toQColor(color));
	painter.setBrush(brush);
}

void BaseCanvas::setLineColor(const Gfx::Color &color)
{
	linePen = colorToPen(color);
	painter.setPen(linePen);
}

void BaseCanvas::setLineWidth(double width)
{
	linePen.setWidthF(width);
	painter.setPen(linePen);
}

void BaseCanvas::beginPolygon()
{
	painter.setPen(linePen);
	polygon = QPainterPath();
}

void BaseCanvas::addPoint(const Geom::Point &point)
{
	if (polygon.elementCount() == 0)
		polygon.moveTo(toQPoint(point));
	else
		polygon.lineTo(toQPoint(point));
}

void BaseCanvas::addBezier(const Geom::Point &control0,
    const Geom::Point &control1,
    const Geom::Point &endPoint)
{
	polygon.cubicTo(toQPoint(control0),
	    toQPoint(control1),
	    toQPoint(endPoint));
}

void BaseCanvas::endPolygon()
{
	painter.drawPath(polygon);
	polygon = QPainterPath();
}

void BaseCanvas::setClipRect(const Geom::Rect &rect)
{
	painter.setClipping(true);
	painter.setClipRect(toQRect(rect));
}

void BaseCanvas::setClipCircle(const Geom::Circle &circle)
{
	QPainterPath path;
	path.addEllipse(toQRect(circle.boundary()));
	painter.setClipPath(path);
}

void BaseCanvas::setClipPolygon()
{
	painter.setClipping(true);
	painter.setClipPath(polygon);
	polygon = QPainterPath();
}

void BaseCanvas::setFont(const Gfx::Font &newFont)
{
	painter.setFont(fromGfxFont(newFont, painter.font()));
}

void BaseCanvas::setTextColor(const Gfx::Color &color)
{
	textPen = colorToPen(color);
	painter.setPen(textPen);
}

void BaseCanvas::beginDropShadow() {}

void BaseCanvas::setDropShadowBlur(double) {}

void BaseCanvas::setDropShadowColor(const Gfx::Color &) {}

void BaseCanvas::setDropShadowOffset(const Geom::Point &) {}

void BaseCanvas::endDropShadow() {}

void BaseCanvas::circle(const Geom::Circle &circle)
{
	painter.setPen(linePen);
	painter.drawEllipse(toQPoint(circle.center),
	    circle.radius,
	    circle.radius);
}

void BaseCanvas::line(const Geom::Line &line)
{
	painter.drawLine(toQLine(line));
}

void BaseCanvas::rectangle(const Geom::Rect &rect)
{
	painter.setPen(linePen);
	painter.drawRect(toQRect(rect));
}

void BaseCanvas::text(const Geom::Rect &rect, const char *text)
{
	painter.setPen(textPen);
	painter.drawText(toQRect(rect),
	    Qt::AlignLeft,
	    QString::fromStdString(text));
}

void BaseCanvas::setBrushGradient(const Geom::Line &line,
    const Gfx::ColorGradient &gradient)
{
	QLinearGradient qGradient(toQPoint(line.begin),
	    toQPoint(line.end));
	for (auto stop : gradient.stops) {
		qGradient.setColorAt(stop.pos, toQColor(stop.value));
	}
	painter.setBrush(QBrush(qGradient));
}

QPen BaseCanvas::colorToPen(const Gfx::Color &color)
{
	return brushToPen(QBrush(toQColor(color)));
}

QPen BaseCanvas::brushToPen(const QBrush &brush)
{
	auto pen = painter.pen();
	pen.setBrush(brush);
	return pen;
}

Geom::Size Gfx::ICanvas::textBoundary(const Gfx::Font &font,
    const char *text)
{
	auto res =
	    QFontMetrics{BaseCanvas::fromGfxFont(font)}.boundingRect(
	        QRect(0, 0, 0, 0),
	        Qt::AlignLeft,
	        QString::fromStdString(text));

	return {static_cast<double>(res.width()),
	    static_cast<double>(res.height())};
}

void BaseCanvas::transform(const Geom::AffineTransform &transform)
{
	const auto &[r0, r1] = transform.getMatrix();
	painter.setTransform(QTransform(r0[0],
	    r1[0],
	    0,
	    r0[1],
	    r1[1],
	    0,
	    r0[2],
	    r1[2],
	    1));
}

void BaseCanvas::save() { painter.save(); }

void BaseCanvas::restore() { painter.restore(); }

QFont BaseCanvas::fromGfxFont(const Gfx::Font &newFont, QFont font)
{
	font.setPixelSize(static_cast<int>(newFont.size));

	if (!newFont.family.empty())
		font.setFamily(
		    QString::fromStdString(newFont.family.toString()));

	font.setWeight(newFont.weight == Gfx::Font::Weight::Bold()
	                   ? QFont::Bold
	               : newFont.weight == Gfx::Font::Weight::Normal()
	                   ? QFont::Normal
	                   : static_cast<int>(newFont.weight) / 10);

	font.setStyle(newFont.style == Gfx::Font::Style::italic
	                  ? QFont::StyleItalic
	              : newFont.style == Gfx::Font::Style::oblique
	                  ? QFont::StyleOblique
	                  : QFont::StyleNormal);
	return font;
}