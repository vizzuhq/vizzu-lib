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

void vizzu_init(double dpi, double width_mm, double height_mm)
{
	Interface::instance.init(dpi, width_mm, height_mm);
}

void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift)
{
	Interface::instance.keyPress(key, ctrl, alt, shift);
}

void vizzu_mouseMove(double x, double y)
{
	Interface::instance.mouseMove(x, y);
}

void vizzu_mouseDown(double x, double y)
{
	Interface::instance.mouseDown(x, y);
}

void vizzu_mouseUp(double x, double y)
{
	Interface::instance.mouseUp(x, y);
}

void vizzu_poll()
{
	Interface::instance.poll();
}

void vizzu_update(double scale, double width, double height, bool force)
{
	Interface::instance.update(scale, width, height, force);
}

void style_setValue(const char *path, const char *value)
{
	Interface::instance.setStyleValue(path, value);
}

void chart_setValue(const char *path, const char *value)
{
	Interface::instance.setChartValue(path, value);
}

void data_addCategories(const char *name,
    const char **categories,
    int count)
{
	Interface::instance.addCategories(name, categories, count);
}

void data_addValues(const char *name, double *values, int count)
{
	Interface::instance.addValues(name, values, count);
}

void chart_animate(void (*callback)())
{
	Interface::instance.animate(callback);
}

void anim_control(const char *command, const char *param)
{
	Interface::instance.animControl(command, param);
}
