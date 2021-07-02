#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include "chart/main/version.h"
#include "chart/ui/chart.h"

namespace Vizzu
{

class Interface
{
public:
	static Interface instance;

	Interface();
	const char *version() const;
	void init(double dpi, double width_mm, double height_mm);
	void setLogging(bool enable) { logging = enable; }
	void keyPress(int key, bool ctrl, bool alt, bool shift);
	void mouseMove(double x, double y);
	void mouseDown(double x, double y);
	void mouseUp(double x, double y);
	void update(double scale, double width, double height, bool force);
	void poll();

	void setStyleValue(const char *path, const char *value);
	void setChartValue(const char *path, const char *value);
	void setChartFilter(bool (*filter)(const void *));
	void addCategories(const char *name, const char **categories, int count);
	void addValues(const char *name, double *values, int count);
	int addEventListener(const char *name);
	void removeEventListener(const char *name, int id);
	void preventDefaultEvent();
	void animate(void (*callback)());
	void animControl(const char *command, const char *param);

	static const void *
	getRecordValue(void *record, const char *column, bool discrete);

private:
	std::string versionStr;
	std::shared_ptr<GUI::TaskQueue> taskQueue;
	std::shared_ptr<UI::ChartWidget> chart;
	Util::EventDispatcher::Params *eventParam;
	bool needsUpdate;
	bool logging;
	void log(const char* str);
};

}

#endif
