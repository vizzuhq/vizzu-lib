#ifndef QT_WINDOW_H
#define QT_WINDOW_H

#include <QMainWindow>

#include "apps/qutils/qtscheduler.h"

#include "chart.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
	Q_OBJECT

public:
	explicit Window(QWidget *parent = nullptr);
	~Window() override;
	void paintEvent(QPaintEvent *) override;

private slots:

private:
	Ui::Window *ui;
	std::shared_ptr<QtScheduler> scheduler;
	TestChart chart;
	void animStep();
};

#endif
