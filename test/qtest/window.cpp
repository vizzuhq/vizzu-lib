
#include "window.h"

#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

#include "apps/qutils/canvas.h"

#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    chart(),
    ui(std::make_unique<Ui::Window>())
{
	ui->setupUi(this);
	chart.getChart().doChange = [this]()
	{
		update();
	};
	resize(640, 480);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::white);
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
#ifndef __clang_analyzer__
	return QTimer::singleShot(25,
	    [this]
	    {
		    animStep();
	    });
#endif
}

Window::~Window() = default;

void Window::paintEvent(QPaintEvent *)
{
	Canvas canvas(this);
	const Geom::Size size{static_cast<double>(width()),
	    static_cast<double>(height())};

	chart.getChart().onUpdateSize(size);
	canvas.frameBegin();
	chart.getChart().onDraw({std::shared_ptr<void>{}, &canvas});
	canvas.frameEnd();
}

bool Window::eventFilter(QObject *, QEvent *event)
{
	try {
		auto type = event->type();
		if (type == QEvent::MouseButtonPress) {
			auto *e = static_cast<QMouseEvent *>(event);
			const Geom::Point pos(e->x(), e->y());
			chart.getChart().onPointerDown({0, pos});
			return true;
		}
		if (type == QEvent::MouseButtonRelease) {
			auto *e = static_cast<QMouseEvent *>(event);
			const Geom::Point pos(e->x(), e->y());
			chart.getChart().onPointerUp({0, pos});
			return true;
		}
		if (type == QEvent::HoverMove) {
			auto *e = static_cast<QHoverEvent *>(event);
			const Geom::Point pos(e->pos().x(), e->pos().y());
			chart.getChart().onPointerMove({0, pos});
			return true;
		}
		if (type == QEvent::HoverLeave) {
			chart.getChart().onPointerLeave(
			    {0, Geom::Point::Invalid()});
			return true;
		}
	}
	catch (std::exception const &x) {
		if (x.what()
		    != std::string_view{"animation already in progress"})
			printf("Exception thrown at mouse event: %s(\"%s\")\n",
			    typeid(x).name() + 4,
			    x.what());
	}
	catch (...) {
		printf("Unknown exception thrown at mouse event.\n");
	}
	return false;
}
void Window::resizeEvent(QResizeEvent *) { update(); }
