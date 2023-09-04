#ifndef LIB_CINTERFACE_H
#define LIB_CINTERFACE_H

#include <cstdint>
#include <typeinfo>

extern "C" {

struct alignas(double) Point {
    double x;
    double y;
};

extern void vizzu_init();
extern void vizzu_poll();
extern void vizzu_pointerDown(int pointerId, double x, double y);
extern void vizzu_pointerUp(int pointerId, double x, double y);
extern void vizzu_pointerMove(int pointerId, double x, double y);
extern void vizzu_pointerLeave(int pointerId);
extern void vizzu_wheel(double delta);
extern void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift);
extern void vizzu_setLogging(bool enable);
extern void
vizzu_update(double width, double height, int renderControl);
extern const char *vizzu_errorMessage(const void *exceptionPtr,
    const std::type_info *typeinfo);
extern const char *vizzu_version();

extern void data_addDimension(const char *name,
    const char **categories,
    int count);
extern void
data_addMeasure(const char *name, double *values, int count);
extern void data_addRecord(const char **cells, int count);
const char *data_metaInfo();

extern const void *
record_getValue(void *record, const char *column, bool isDimension);
extern void *chart_store();
extern void chart_restore(void *chart);
extern void *chart_anim_store();
extern void chart_anim_restore(void *anim);
extern void object_free(void *ptr);
extern const char *style_getList();
extern void style_setValue(const char *path, const char *value);
extern const char *style_getValue(const char *path, bool computed);
const char *chart_getList();
const char *chart_getValue(const char *path);
extern void chart_setValue(const char *path, const char *value);
extern void chart_setFilter(bool (*)(const void *),
    void (*)(bool (*)(const void *)));
extern void chart_animate(void (*callback)(bool));
extern const Point*
chart_relToCanvasCoords(double rx, double ry);
extern const Point*
chart_canvasToRelCoords(double x, double y);
extern void chart_setKeyframe();
const char *chart_markerData(unsigned id);
extern void addEventListener(const char *name,
    void (*callback)(const char *));
extern void removeEventListener(const char *name,
    void (*callback)(const char *));
extern void event_preventDefault();
extern void anim_control(const char *command, const char *param);
extern void anim_setValue(const char *path, const char *value);
}

#endif
