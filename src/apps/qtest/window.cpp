
#include "window.h"

#include <QApplication>

#include "apps/qutils/canvas.h"

#include "ui_window.h"

using namespace Vizzu;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window),
    scheduler(std::make_shared<QtScheduler>()),
	chart()
{
	ui->setupUi(this);

	chart.getChart().doChange = [=]() { update(); };

	resize(640, 480);

	QPalette pal = palette();
	pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);

	chart.run();
	animStep();
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
