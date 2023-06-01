#include "cinterface.h"
#include "interface.h"

using namespace Vizzu;

const char *vizzu_errorMessage(int exceptionPtr)
{
	return reinterpret_cast<std::exception*>(exceptionPtr)->what();
}

extern const char *vizzu_version()
{
	return Interface::instance.version();
}

void vizzu_setLogging(bool enable)
{
	Interface::instance.setLogging(enable);
}

void vizzu_init()
{
	Interface::instance.init();
}

void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift)
{
	Interface::instance.keyPress(key, ctrl, alt, shift);
}

void vizzu_pointerMove(int pointerId, double x, double y)
{
	Interface::instance.pointerMove(pointerId, x, y);
}

void vizzu_pointerDown(int pointerId, double x, double y)
{
	Interface::instance.pointerDown(pointerId, x, y);
}

void vizzu_pointerUp(int pointerId, double x, double y)
{
	Interface::instance.pointerUp(pointerId, x, y);
}

void vizzu_pointerLeave(int pointerId)
{
	Interface::instance.pointerLeave(pointerId);
}

void vizzu_wheel(double delta)
{
	Interface::instance.wheel(delta);
}

void vizzu_poll()
{
	Interface::instance.poll();
}

void vizzu_update(double width, double height, int renderControl)
{
	Interface::instance
	.update(width, height, (Interface::RenderControl)renderControl);
}

const char *style_getList()
{
	return Interface::instance.getStyleList();
}

void style_setValue(const char *path, const char *value)
{
	Interface::instance.setStyleValue(path, value);
}

const char *style_getValue(const char *path, bool computed)
{
	return Interface::instance.getStyleValue(path, computed);
}

void *chart_store()
{
	return Interface::instance.storeChart();
}

void chart_restore(void *chart)
{
	Interface::instance.restoreChart(chart);
}

void *chart_anim_store()
{
	return Interface::instance.storeAnim();
}

void chart_anim_restore(void *anim)
{
	Interface::instance.restoreAnim(anim);
}

void object_free(void *ptr)
{
	Interface::instance.freeObj(ptr);
}

const char *chart_getList()
{
	return Interface::instance.getChartParamList();
}

const char *chart_getValue(const char *path)
{
	return Interface::instance.getChartValue(path);
}

void chart_setValue(const char *path, const char *value)
{
	Interface::instance.setChartValue(path, value);
}

void chart_setFilter(bool (*filter)(const void *))
{
	Interface::instance.setChartFilter(filter);
}

const void *record_getValue(void *record, const char *column, bool discrete)
{
	return Interface::instance.getRecordValue(record, column, discrete);
}

void data_addDimension(const char *name,
    const char **categories,
    int count)
{
	Interface::instance.addDimension(name, categories, count);
}

void data_addMeasure(const char *name, double *values, int count)
{
	Interface::instance.addMeasure(name, values, count);
}

void data_addRecord(const char **cells, int count)
{
	Interface::instance.addRecord(cells, count);
}

const char *data_metaInfo()
{
	return Interface::instance.dataMetaInfo();
}

int addEventListener(const char *name) {
	return Interface::instance.addEventListener(name);
}

void removeEventListener(const char *name, int id) {
	Interface::instance.removeEventListener(name, id);
}

void event_preventDefault()
{
	Interface::instance.preventDefaultEvent();
}

void chart_animate(void (*callback)(bool))
{
	Interface::instance.animate(callback);
}

void chart_relToCanvasCoords(double rx, double ry, double *x, double *y)
{
	Interface::instance.relToCanvasCoords(rx, ry, *x, *y);
}

void chart_canvasToRelCoords(double x, double y, double *rx, double *ry)
{
	Interface::instance.canvasToRelCoords(x, y, *rx, *ry);
}

void chart_setKeyframe()
{
	Interface::instance.setKeyframe();
}

const char *chart_markerData(unsigned id)
{
	return Interface::instance.getMarkerData(id);
}

void anim_control(const char *command, const char *param)
{
	Interface::instance.animControl(command, param);
}

void anim_setValue(const char *path, const char *value)
{
	Interface::instance.setAnimValue(path, value);
}
