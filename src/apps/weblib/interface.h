#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include "chart/main/version.h"
#include "chart/ui/chart.h"

#include "jsfunctionwrapper.h"
#include "objectregistry.h"

namespace Vizzu
{

class Interface
{
public:
	static Interface &getInstance();

	enum RenderControl { allow = 0, force = 1, inhibit = 2 };

	Interface();
	static const char *version();
	ObjectRegistry::Handle createChart();
	static void setLogging(bool enable);
	void keyPress(int key, bool ctrl, bool alt, bool shift);
	void pointerMove(int pointerId, double x, double y);
	void pointerDown(int pointerId, double x, double y);
	void pointerUp(int pointerId, double x, double y);
	void pointerLeave(int pointerId);
	void wheel(double delta);
	void
	update(double width, double height, RenderControl renderControl);

	ObjectRegistry::Handle storeAnim();
	void restoreAnim(ObjectRegistry::Handle anim);
	ObjectRegistry::Handle storeChart();
	void restoreChart(ObjectRegistry::Handle chart);
	void freeObj(ObjectRegistry::Handle ptr);

	static const char *getStyleList();
	const char *getStyleValue(const char *path, bool computed);
	void setStyleValue(const char *path, const char *value);

	static const char *getChartParamList();
	const char *getChartValue(const char *path);
	void setChartValue(const char *path, const char *value);
	void setChartFilter(
	    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter);

	void
	relToCanvasCoords(double rx, double ry, double &x, double &y);
	void
	canvasToRelCoords(double x, double y, double &rx, double &ry);

	void addDimension(const char *name,
	    const char **categories,
	    int count);
	void addMeasure(const char *name, double *values, int count);
	void addRecord(const char **cells, int count);
	const char *dataMetaInfo();
	void addEventListener(const char *event,
	    void (*callback)(ObjectRegistry::Handle, const char *));
	void removeEventListener(const char *event,
	    void (*callback)(ObjectRegistry::Handle, const char *));
	void preventDefaultEvent(ObjectRegistry::Handle);
	void animate(void (*callback)(bool));
	void setKeyframe();
	const char *getMarkerData(unsigned id);
	void animControl(const char *command, const char *param);
	void setAnimValue(const char *path, const char *value);

	static std::variant<const double *, const char *> getRecordValue(
	    const Data::RowWrapper &record,
	    const char *column,
	    bool isDimension);

private:
	struct Snapshot
	{
		Snapshot(Gen::Options options, Styles::Chart styles) :
		    options(std::move(options)),
		    styles(std::move(styles))
		{}
		Gen::Options options;
		Styles::Chart styles;
	};

	struct Animation
	{
		Animation(Anim::AnimationPtr anim, Snapshot snapshot) :
		    animation(std::move(anim)),
		    snapshot(std::move(snapshot))
		{}
		Anim::AnimationPtr animation;
		Snapshot snapshot;
	};

	struct CScheduler : GUI::Scheduler
	{
		struct ScheduledTask
		{
			Task task;
			CScheduler *scheduler;
			std::list<ScheduledTask>::iterator it;
		};

		void schedule(const Task &task,
		    std::chrono::steady_clock::time_point time) final;

		std::list<ScheduledTask> tasks;
		std::mutex mutex;
	};

	CScheduler scheduler;
	ObjectRegistry objects;
	std::shared_ptr<UI::ChartWidget> widget;
	std::shared_ptr<Vizzu::Chart> chart;
};

}

#endif
