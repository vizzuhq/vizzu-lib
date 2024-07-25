#ifndef LIB_INTERFACE_H
#define LIB_INTERFACE_H

#include "cinterface.h"
#include "jsfunctionwrapper.h"
#include "objectregistry.h"

namespace Gfx
{
struct ICanvas;
}

namespace Vizzu
{
namespace UI
{
class ChartWidget;
}

class Chart;

class Interface
{
public:
	static Interface &getInstance();

	Interface();
	static const char *version();
	ObjectRegistryHandle createChart();
	ObjectRegistryHandle createCanvas();
	static void setLogging(bool enable);
	void pointerMove(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerDown(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerUp(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    int pointerId,
	    double x,
	    double y);
	void pointerLeave(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    int pointerId);
	void wheel(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    double delta);
	void update(ObjectRegistryHandle chart, double timeInMSecs);
	void render(ObjectRegistryHandle chart,
	    ObjectRegistryHandle canvas,
	    double width,
	    double height);
	void setLineResolution(ObjectRegistryHandle canvas,
	    double distanceMax,
	    double curveHeightMax);

	ObjectRegistryHandle storeAnim(ObjectRegistryHandle chart);
	void restoreAnim(ObjectRegistryHandle chart,
	    ObjectRegistryHandle anim);
	ObjectRegistryHandle storeChart(ObjectRegistryHandle chart);
	void restoreChart(ObjectRegistryHandle chart,
	    ObjectRegistryHandle snapshot);
	void freeObj(ObjectRegistryHandle ptr);

	static const char *getStyleList();
	const char *getStyleValue(ObjectRegistryHandle chart,
	    const char *path,
	    bool computed);
	void setStyleValue(ObjectRegistryHandle chart,
	    const char *path,
	    const char *value);

	static const char *getChartParamList();
	const char *getChartValue(ObjectRegistryHandle chart,
	    const char *path);
	void setChartValue(ObjectRegistryHandle chart,
	    const char *path,
	    const char *value);
	void setChartFilter(ObjectRegistryHandle chart,
	    JsFunctionWrapper<bool, const Data::RowWrapper &> &&filter);

	void relToCanvasCoords(ObjectRegistryHandle chart,
	    double rx,
	    double ry,
	    double &x,
	    double &y);
	void canvasToRelCoords(ObjectRegistryHandle chart,
	    double x,
	    double y,
	    double &rx,
	    double &ry);

	void addDimension(ObjectRegistryHandle chart,
	    const char *name,
	    const char **categories,
	    std::uint32_t categoriesCount,
	    const std::uint32_t *categoryIndices,
	    std::uint32_t categoryIndicesCount);
	void addMeasure(ObjectRegistryHandle chart,
	    const char *name,
	    const char *unit,
	    const double *values,
	    std::uint32_t count);
	void addRecord(ObjectRegistryHandle chart,
	    const char *const *cells,
	    std::uint32_t count);
	const char *dataMetaInfo(ObjectRegistryHandle chart);
	void addEventListener(ObjectRegistryHandle chart,
	    const char *event,
	    void (*callback)(APIHandles::Event, const char *));
	void removeEventListener(ObjectRegistryHandle chart,
	    const char *event,
	    void (*callback)(APIHandles::Event, const char *));
	static void preventDefaultEvent(APIHandles::Event);
	void animate(ObjectRegistryHandle chart, void (*callback)(bool));
	void setKeyframe(ObjectRegistryHandle chart);
	void setAnimControlValue(ObjectRegistryHandle chart,
	    std::string_view path,
	    const char *value);
	const char *getAnimControlValue(ObjectRegistryHandle chart,
	    std::string_view path);
	void setAnimValue(ObjectRegistryHandle chart,
	    const char *path,
	    const char *value);

	static std::variant<double, const std::string *> getRecordValue(
	    const Data::RowWrapper &record,
	    const char *column);

private:
	struct Snapshot;
	struct Animation;

	std::shared_ptr<Chart> getChart(ObjectRegistryHandle chart);

	ObjectRegistry<Snapshot, Animation, Gfx::ICanvas, UI::ChartWidget>
	    objects;
};

}

#endif
