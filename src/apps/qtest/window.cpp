
#include "window.h"

#include <QApplication>
#include <QMouseEvent>

#include "apps/qutils/canvas.h"

#include "ui_window.h"

using namespace Vizzu;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
	scheduler(std::make_shared<QtScheduler>()),
	chart(scheduler),
    ui(new Ui::Window)
{
	ui->setupUi(this);
	chart.getChart().doChange = [=]() {
		update();
	};
	resize(640, 480);
	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
	chart.run();
	animStep();
	installEventFilter(this);
}

void Window::animStep()
{
	auto now = std::chrono::steady_clock::now();
	chart.getChart().getChart().getAnimControl().update(now);
	scheduler->schedule([&]{ animStep(); }, now + std::chrono::milliseconds(25));
}

Window::~Window()
{
	delete ui;
}

void Window::paintEvent(QPaintEvent *)
{
	Canvas canvas(this);
	Geom::Size size(width(), height());

	chart.getChart().updateSize(canvas, size);
	canvas.frameBegin();
	chart.getChart().draw(canvas);
	canvas.frameEnd();
}

bool Window::eventFilter(QObject *, QEvent *event) {
	GUI::DragObjectPtr nodrag;
	auto type = event->type();
	if (type == QEvent::MouseButtonPress) {
		QMouseEvent* e = static_cast<QMouseEvent*>(event);
		Geom::Point pos(e->x(), e->y());
		chart.getChart().onPointerDown(GUI::PointerEvent(0, pos));
		return true;
	}
	if (type == QEvent::MouseButtonRelease) {
		QMouseEvent* e = static_cast<QMouseEvent*>(event);
		Geom::Point pos(e->x(), e->y());
		chart.getChart().onPointerUp(GUI::PointerEvent(0, pos), nodrag);
		return true;
	}
	if (type == QEvent::HoverMove) {
		QHoverEvent* e = static_cast<QHoverEvent*>(event);
		Geom::Point pos(e->pos().x(), e->pos().y());
		chart.getChart().onPointerMove(GUI::PointerEvent(0, pos), nodrag);
		return true;
	}
	if (type == QEvent::HoverLeave) {
		chart.getChart().onPointerLeave(0);
		return true;
	}
	return false;
}