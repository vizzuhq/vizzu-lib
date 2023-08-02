#ifndef QT_WINDOW_H
#define QT_WINDOW_H

#include <QMainWindow>

#include "apps/qutils/qtscheduler.h"

#include "chart.h"

namespace Ui
{
class Window;
}

class Window : public QMainWindow
{
	Q_OBJECT

public:
	explicit Window(QWidget *parent = nullptr);
	~Window() override;
	void paintEvent(QPaintEvent *) override;

private:
	std::shared_ptr<QtScheduler> scheduler;
	TestChart chart;
	std::unique_ptr<Ui::Window> ui;

	void animStep();
	bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif
