#ifndef QTEST_CHART_H
#define QTEST_CHART_H

#include "chart/ui/chart.h"

class TestChart
{
public:
	TestChart();
	void run();
	Vizzu::UI::ChartWidget &getChart() { return chart; }

private:
	Vizzu::UI::ChartWidget chart;
	void prepareData();
};

#endif
