#ifndef LIB_CINTERFACE_H
#define LIB_CINTERFACE_H

#include <cstdint>
#include <typeinfo>

namespace Vizzu::Data
{
class RowWrapper;
}

namespace APIHandles
{
using Any = const void *;
using Chart = const void *;
using Snapshot = const void *;
using Event = const void *;
using Animation = const void *;
using Exception = const void *;
using Canvas = const void *;
}

extern "C" {

struct alignas(double) Point
{
	double x;
	double y;
};

struct alignas(double) Value
{
	bool dimension;
	union
	{
		double measureValue;
		const char *dimensionValue;
	};
};

extern APIHandles::Chart vizzu_createChart();
extern APIHandles::Canvas vizzu_createCanvas();
extern void vizzu_pointerDown(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerUp(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerMove(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerLeave(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId);
extern void vizzu_wheel(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double delta);
extern void vizzu_setLogging(bool enable);
extern void vizzu_update(APIHandles::Chart chart, double timeInMSecs);
extern void vizzu_render(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double width,
    double height);
extern const char *vizzu_errorMessage(
    APIHandles::Exception exceptionPtr,
    const std::type_info *typeinfo);
extern const char *vizzu_version();

extern void data_addDimension(APIHandles::Chart chart,
    const char *name,
    const char **categories,
    std::uint32_t categoriesCount,
    const std::uint32_t *categoryIndices,
    std::uint32_t categoryIndicesCount);
extern void data_addMeasure(APIHandles::Chart chart,
    const char *name,
    const char *unit,
    const double *values,
    std::uint32_t count);
extern void data_addRecord(APIHandles::Chart chart,
    const char **cells,
    std::uint32_t count);
const char *data_metaInfo(APIHandles::Chart chart);

extern const Value *record_getValue(
    const Vizzu::Data::RowWrapper *record,
    const char *column);
extern APIHandles::Snapshot chart_store(APIHandles::Chart chart);
extern void chart_restore(APIHandles::Chart chart,
    APIHandles::Snapshot snapshot);
extern APIHandles::Animation chart_anim_store(
    APIHandles::Chart chart);
extern void chart_anim_restore(APIHandles::Chart chart,
    APIHandles::Animation anim);
extern void object_free(APIHandles::Any handle);
extern const char *style_getList();
extern void style_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value);
extern const char *style_getValue(APIHandles::Chart chart,
    const char *path,
    bool computed);
const char *chart_getList();
const char *chart_getValue(APIHandles::Chart chart, const char *path);
extern void chart_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value);
extern void chart_setFilter(APIHandles::Chart chart,
    bool (*)(const Vizzu::Data::RowWrapper *),
    void (*)(bool (*)(const Vizzu::Data::RowWrapper *)));
extern void chart_animate(APIHandles::Chart chart,
    void (*callback)(bool));
extern const Point *chart_relToCanvasCoords(APIHandles::Chart chart,
    double rx,
    double ry);
extern const Point *
chart_canvasToRelCoords(APIHandles::Chart chart, double x, double y);
extern void chart_setKeyframe(APIHandles::Chart chart);
extern void addEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *));
extern void removeEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *));
extern void event_preventDefault(APIHandles::Event event);
extern void anim_control_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value);
extern const char *anim_control_getValue(APIHandles::Chart chart,
    const char *path);
extern void anim_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value);
}

#endif
