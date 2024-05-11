#ifndef QTEST_CHART_H
#define QTEST_CHART_H

#include "chart/ui/chart.h"

class TestChart
{
public:
	explicit TestChart();
	void run();
	Vizzu::UI::ChartWidget &getChart() { return chart; }
	void operator()(Util::EventDispatcher::Params &param,
	    const std::string &);

private:
	Vizzu::UI::ChartWidget chart;
	void prepareData();
};

#endif
