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
	ObjectRegistry::Handle createCanvas();
	static void setLogging(bool enable);
	void pointerMove(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerDown(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerUp(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerLeave(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    int pointerId);
	void wheel(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    double delta);
	void update(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle canvas,
	    double width,
	    double height,
	    double timeInMSecs,
	    RenderControl renderControl);

	ObjectRegistry::Handle storeAnim(ObjectRegistry::Handle chart);
	void restoreAnim(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle anim);
	ObjectRegistry::Handle storeChart(ObjectRegistry::Handle chart);
	void restoreChart(ObjectRegistry::Handle chart,
	    ObjectRegistry::Handle snapshot);
	void freeObj(ObjectRegistry::Handle ptr);

	static const char *getStyleList();
	const char *getStyleValue(ObjectRegistry::Handle chart,
	    const char *path,
	    bool computed);
	void setStyleValue(ObjectRegistry::Handle chart,
	    const char *path,
	    const char *value);

	static const char *getChartParamList();
	const char *getChartValue(ObjectRegistry::Handle chart,
	    const char *path);
	void setChartValue(ObjectRegistry::Handle chart,
	    const char *path,
	    const char *value);
	void setChartFilter(ObjectRegistry::Handle chart,
	    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter);

	void relToCanvasCoords(ObjectRegistry::Handle chart,
	    double rx,
	    double ry,
	    double &x,
	    double &y);
	void canvasToRelCoords(ObjectRegistry::Handle chart,
	    double x,
	    double y,
	    double &rx,
	    double &ry);

	void addDimension(ObjectRegistry::Handle chart,
	    const char *name,
	    const char **categories,
	    std::uint32_t categoriesCount,
	    const std::uint32_t *categoryIndices,
	    std::uint32_t categoryIndicesCount);
	void addMeasure(ObjectRegistry::Handle chart,
	    const char *name,
	    const char *unit,
	    const double *values,
	    std::uint32_t count);
	void addRecord(ObjectRegistry::Handle chart,
	    const char **cells,
	    std::uint32_t count);
	const char *dataMetaInfo(ObjectRegistry::Handle chart);
	void addEventListener(ObjectRegistry::Handle chart,
	    const char *event,
	    void (*callback)(ObjectRegistry::Handle, const char *));
	void removeEventListener(ObjectRegistry::Handle chart,
	    const char *event,
	    void (*callback)(ObjectRegistry::Handle, const char *));
	void preventDefaultEvent(ObjectRegistry::Handle);
	void animate(ObjectRegistry::Handle chart,
	    void (*callback)(bool));
	void setKeyframe(ObjectRegistry::Handle chart);
	void setAnimControlValue(ObjectRegistry::Handle chart,
	    std::string_view path,
	    const char *value);
	const char *getAnimControlValue(ObjectRegistry::Handle chart,
	    std::string_view path);
	void setAnimValue(ObjectRegistry::Handle chart,
	    const char *path,
	    const char *value);

	static std::variant<const char *, double> getRecordValue(
	    const Data::RowWrapper &record,
	    const char *column);

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

	std::shared_ptr<Vizzu::Chart> getChart(
	    ObjectRegistry::Handle chart);

	ObjectRegistry objects;
};

}

#endif
