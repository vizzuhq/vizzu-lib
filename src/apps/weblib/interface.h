#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include <unordered_map>

#include "chart/main/version.h"
#include "chart/ui/chart.h"

namespace Vizzu
{

class ObjectRegistry
{
public:
	typedef void* Handle;

	Handle reg(std::shared_ptr<void> ptr) {
		Handle handle = ptr.get();
		objects.emplace(handle, ptr);
		return handle;
	}

	template <class T>
	std::shared_ptr<T> get(Handle handle)
	{
		auto it = objects.find(handle);
		if (it == objects.end() || !it->second) 
			throw std::logic_error("No such object exists");
		return std::static_pointer_cast<T>(it->second);
	}

	void unreg(Handle handle)
	{
		auto it = objects.find(handle);
		if (it == objects.end()) throw std::logic_error("No such object exists");
		objects.erase(it);
	}

private:
	std::unordered_map<void*, std::shared_ptr<void>> objects;
};

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

	void *storeAnim();
	void restoreAnim(void *anim);
	void *storeChart();
	void restoreChart(void *chart);
	void freeObj(void *ptr);
	const char *getStyleList();
	const char *getStyleValue(const char *path, bool computed);
	void setStyleValue(const char *path, const char *value);
	const char *getChartParamList();
	const char *getChartValue(const char *path);
	void setChartValue(const char *path, const char *value);
	void setChartFilter(bool (*filter)(const void *));
	void addDimension(const char *name, const char **categories, int count);
	void addMeasure(const char *name, double *values, int count);
	void addRecord(const char **cells, int count);
	const char *dataMetaInfo();
	int addEventListener(const char *name);
	void removeEventListener(const char *name, int id);
	void preventDefaultEvent();
	void animate(void (*callback)(bool));
	void setKeyframe();
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

	struct Animation {
		Animation(Anim::AnimationPtr anim, Snapshot snapshot) :
			animation(anim), snapshot(snapshot)
		{}
		Anim::AnimationPtr animation;
		Snapshot snapshot;
	};

	std::string versionStr;
	std::shared_ptr<GUI::TaskQueue> taskQueue;
	std::shared_ptr<UI::ChartWidget> chart;
	ObjectRegistry objects;
	Util::EventDispatcher::Params *eventParam;
	bool needsUpdate;
	bool logging;
	void log(const char* str);
};

}

#endif
