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

extern void callback(void (*task)(void *), void *obj);

extern APIHandles::Chart vizzu_createChart();
extern APIHandles::Canvas vizzu_createCanvas();
extern void vizzu_pointerDown(APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerUp(APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerMove(APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y);
extern void vizzu_pointerLeave(APIHandles::Canvas canvas,
    int pointerId);
extern void vizzu_wheel(APIHandles::Canvas canvas, double delta);
extern void vizzu_keyPress(APIHandles::Canvas canvas,
    int key,
    bool ctrl,
    bool alt,
    bool shift);
extern void vizzu_setLogging(bool enable);
extern void vizzu_update(APIHandles::Canvas canvas,
    double width,
    double height,
    int renderControl);
extern const char *vizzu_errorMessage(
    APIHandles::Exception exceptionPtr,
    const std::type_info *typeinfo);
extern const char *vizzu_version();

extern void data_addDimension(const char *name,
    const char **categories,
    int count);
extern void data_addMeasure(const char *name,
    const char *unit,
    double *values,
    int count);
extern void data_addRecord(const char **cells, int count);
const char *data_metaInfo();

extern const Value *record_getValue(
    const Vizzu::Data::RowWrapper *record,
    const char *column);
extern APIHandles::Snapshot chart_store();
extern void chart_restore(APIHandles::Snapshot chart);
extern APIHandles::Animation chart_anim_store();
extern void chart_anim_restore(APIHandles::Animation anim);
extern void object_free(APIHandles::Any handle);
extern const char *style_getList();
extern void style_setValue(const char *path, const char *value);
extern const char *style_getValue(const char *path, bool computed);
const char *chart_getList();
const char *chart_getValue(const char *path);
extern void chart_setValue(const char *path, const char *value);
extern void chart_setFilter(bool (*)(const Vizzu::Data::RowWrapper *),
    void (*)(bool (*)(const Vizzu::Data::RowWrapper *)));
extern void chart_animate(void (*callback)(bool));
extern const Point *chart_relToCanvasCoords(double rx, double ry);
extern const Point *chart_canvasToRelCoords(double x, double y);
extern void chart_setKeyframe();
const char *chart_markerData(unsigned id);
extern void addEventListener(const char *name,
    void (*callback)(APIHandles::Event event, const char *));
extern void removeEventListener(const char *name,
    void (*callback)(APIHandles::Event event, const char *));
extern void event_preventDefault(APIHandles::Event event);
extern void anim_control(const char *command, const char *param);
extern void anim_setValue(const char *path, const char *value);
}

#endif
