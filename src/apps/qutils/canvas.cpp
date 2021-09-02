#include "canvas.h"

#include <QDebug>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QLinearGradient>
#include <QScreen>

using namespace Vizzu;

class PixmapCanvas : public Canvas {
public:
	PixmapCanvas(int width, int height)
		: pixmap(width, height)
	{
		pixmap.fill(Qt::transparent);
		init(&pixmap);
	}
	~PixmapCanvas()
	{
		if (painter.isActive()) painter.end();
	}

	const QPixmap &getPixmap() const { return pixmap; }
	
private:
	QPixmap pixmap;
};

static void initFont(QFont &font)
{
	font.setHintingPreference(QFont::PreferFullHinting);
	font.setStyleStrategy(QFont::PreferAntialias);
}

QColor toQColor(const Gfx::Color &color) {
	return QColor(color.getRedByte(),
				  color.getGreenByte(),
				  color.getBlueByte(),
				  color.getAlphaByte());
}

QPointF toQPoint(const Geom::Point &point)
{
	return QPointF(point.x, point.y);
}

QPoint toQPointInt(const Geom::Point &point)
{
	return QPoint((int)point.x, (int)point.y);
}

Geom::Point fromQPointF(const QPointF &point)
{
	return Geom::Point(point.x(), point.y());
}

QLineF toQLine(const Geom::Line &line)
{
	return QLineF(toQPoint(line.begin), toQPoint(line.end));
}

QSizeF toQSize(const Geom::Size &size)
{
	return QSizeF(size.x, size.y);
}

QSize toQSizeInt(const Geom::Size &size)
{
	return QSize((int)size.x, (int)size.y);
}

Geom::Size fromQSizeF(const QSizeF &size)
{
	return Geom::Size(size.width(), size.height());
}


QRectF toQRect(const Geom::Rect &rect)
{
	return QRectF(toQPoint(rect.pos), toQSize(rect.size));
}

QRect toQRectInt(const Geom::Rect &rect)
{
	return QRect(toQPointInt(rect.pos), toQSizeInt(rect.size));
}

Geom::Rect fromQRectF(const QRectF &rect)
{
	return Geom::Rect(fromQPointF(rect.topLeft()), fromQSizeF(rect.size()));
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
	if(!painter.isActive()) painter.begin(device);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	font = painter.font();
	initFont(font);
	painter.setFont(font);

	auto pen = painter.pen();
	pen.setWidth(0);
	painter.setPen(pen);
}

std::shared_ptr<Gfx::ICanvas> BaseCanvas::createCanvas(int width,
    int height)
{
	return std::make_shared<PixmapCanvas>(width, height);
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

Geom::Rect BaseCanvas::getClipRect() const
{
	if (painter.hasClipping())
		return fromQRectF(painter.clipBoundingRect());
	else
		return Geom::Rect(Geom::Point(),
						  Geom::Size(painter.device()->width(),
									 painter.device()->height()));
}

void BaseCanvas::setClipRect(const Geom::Rect &rect)
{
	painter.setClipping(true);
	painter.setClipRect(toQRect(rect));
}

void BaseCanvas::setClipPolygon() {
	painter.setClipping(true);
	painter.setClipPath(polygon);
	polygon = QPainterPath();
}

void BaseCanvas::setFont(const Gfx::Font &newFont)
{
	auto font = painter.font();
	font.setPixelSize((int)newFont.size);

	if (!newFont.family.empty())
		font.setFamily(QString::fromStdString(newFont.family));

	font.setWeight(
		newFont.weight == Gfx::Font::Weight::Bold()	? QFont::Bold :
		newFont.weight == Gfx::Font::Weight::Normal() ? QFont::Normal :
		(int)newFont.weight / 10);

	font.setStyle(
		newFont.style == Gfx::Font::Style::italic ? QFont::StyleItalic :
		newFont.style == Gfx::Font::Style::oblique ? QFont::StyleOblique :
	                                                 QFont::StyleNormal);

	painter.setFont(font);
}
	
void BaseCanvas::setTextColor(const Gfx::Color &color)
{
	textPen = colorToPen(color);
	painter.setPen(textPen);
}

void BaseCanvas::beginDropShadow() {
}

void BaseCanvas::setDropShadowBlur(uint64_t) {
}

void BaseCanvas::setDropShadowColor(const Gfx::Color &) {
}

void BaseCanvas::setDropShadowOffset(const Geom::Point &) {
}

void BaseCanvas::endDropShadow() {
}

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

void BaseCanvas::text(const Geom::Rect &rect,
    const std::string &text)
{
	painter.setPen(textPen);
	painter.drawText(toQRect(rect), Qt::AlignLeft, QString::fromStdString(text));
}

void BaseCanvas::setBrushGradient(const Geom::Line &line, const Gfx::ColorGradient &gradient)
{
	QLinearGradient qGradient(toQPoint(line.begin), toQPoint(line.end));
	for (auto stop: gradient.stops) {
		qGradient.setColorAt(stop.pos, toQColor(stop.value));
	}
	painter.setBrush(QBrush(qGradient));
}

int BaseCanvas::loadSvgImage(const Gfx::Svg &/*svg*/)
{
	return 0;
}

int BaseCanvas::loadPixMapImage(const Gfx::PixMapView &/*pixmap*/)
{
	return 0;
}

void BaseCanvas::dropImage(int /*imageId*/)
{}

void BaseCanvas::drawImage(int /*imageId*/, const Geom::Rect &/*boundary*/, double /*opacity*/)
{
	// todo: separated image loading and drawing through ICanvas interface
	// The imageId parameter comes from the further ImageLoader component.
	// The loading phase must be implemeted there, the drawing part remains
	// in this function.
	/*QImage image;

	if (pixMap.grayscale)
	{
		image = QImage(pixMap.size.x, pixMap.size.y,
					   QImage::Format_ARGB32);
		for (auto y = 0u; y < pixMap.size.y; y++) {
			auto *in = (uint16_t*)pixMap.scanLine(y);
			auto *out = (QRgb*)image.scanLine(y);
			for (auto x = 0u; x < pixMap.size.x; x++)
			{
				uint8_t value = in[x];
				uint8_t alpha = in[x] >> 8;
				out[x] = qRgba(value, value, value, alpha);
			}
		}
	}
	else
	{
		image = QImage((unsigned char*)pixMap.data,
					   (int)pixMap.size.x, (int)pixMap.size.y,
					   QImage::Format_RGBA8888);
	}

	auto qpixmap = QPixmap::fromImage(image);

	if (opacity < 1.0) painter.setOpacity(opacity);

	if ((rotateQuadrantCW % 4) == 0)
	{
		painter.drawPixmap(toQRectInt(boundary), qpixmap);
	}
	else
	{
		QTransform transform;
		transform.rotate(90 * rotateQuadrantCW);
		painter.drawPixmap(toQRectInt(boundary),
						   qpixmap.transformed(transform));
	}

	if (opacity < 1.0) painter.setOpacity(1.0);*/
}

void BaseCanvas::drawCanvas(const Geom::Rect &boundary, const Gfx::ICanvas &canvas)
{
	const PixmapCanvas *pixmap = dynamic_cast<const PixmapCanvas*>(&canvas);
	if (!pixmap) throw std::logic_error("cannot recover pixmap canvas");
	painter.drawPixmap(toQRectInt(boundary), pixmap->getPixmap());
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

Geom::Size BaseCanvas::textBoundary(const std::string &text)
{
	QFontMetrics metrics(painter.font());
	auto res = metrics.boundingRect(QRect(0, 0, 0, 0),
	    Qt::AlignLeft,
	    QString::fromStdString(text));

	return Geom::Size(res.width(), res.height());
}

void BaseCanvas::transform(const Geom::AffineTransform &transform) {
	painter.translate(transform.offset.x, transform.offset.y);
	painter.rotate(180.0 * transform.rotate / M_PI);
	painter.scale(transform.scale, transform.scale);
}

void BaseCanvas::save() {
	painter.save();
}

void BaseCanvas::restore() {
	painter.restore();
}