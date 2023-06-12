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
	std::shared_ptr<QtScheduler> scheduler;
	TestChart chart;
	Ui::Window *ui;
	
	void animStep();
	bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif
