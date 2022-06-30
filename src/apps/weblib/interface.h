#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include <unordered_map>

#include "chart/main/version.h"
#include "chart/ui/chart.h"

namespace Vizzu
{

class Interface
{
public:
	static Interface instance;

	enum RenderControl { 
		allow = 0,
		force = 1, 
		inhibit = 2
	};

	Interface();
	const char *version() const;
	void init();
	void setLogging(bool enable) { logging = enable; }
	void keyPress(int key, bool ctrl, bool alt, bool shift);
	void mouseMove(double x, double y);
	void mouseDown(double x, double y);
	void mouseUp(double x, double y);
	void mouseLeave();
	void mouseWheel(double delta);
	void update(double width, double height, RenderControl renderControl);
	void poll();

	void *storeChart();
	void restoreChart(void *chart);
	void freeChart(void *chart);
	const char *getStyleList();
	const char *getStyleValue(const char *path);
	void setStyleValue(const char *path, const char *value);
	const char *getChartParamList();
	const char *getChartValue(const char *path);
	void setChartValue(const char *path, const char *value);
	void setChartFilter(bool (*filter)(const void *));
	void addDimension(const char *name, const char **categories, int count);
	void addMeasure(const char *name, double *values, int count);
	void addRecord(const char **cells, int count);
	int addEventListener(const char *name);
	void removeEventListener(const char *name, int id);
	void preventDefaultEvent();
	void animate(void (*callback)(bool));
	void animControl(const char *command, const char *param);
	void setAnimValue(const char *path, const char *value);

	static const void *
	getRecordValue(void *record, const char *column, bool discrete);

private:
	struct Snapshot {
		Snapshot(Diag::Options options, Styles::Chart styles)
			: options(std::move(options)), styles(std::move(styles))
		{}
		Diag::Options options;
		Styles::Chart styles;
	};

	std::string versionStr;
	std::shared_ptr<GUI::TaskQueue> taskQueue;
	std::shared_ptr<UI::ChartWidget> chart;
	std::unordered_map<void*, std::shared_ptr<Snapshot>> snapshots;
	Util::EventDispatcher::Params *eventParam;
	bool needsUpdate;
	bool logging;
	void log(const char* str);
};

}

#endif
