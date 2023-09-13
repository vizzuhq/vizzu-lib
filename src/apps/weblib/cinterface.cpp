#include "cinterface.h"

#include "interface.h"

using namespace Vizzu;

static_assert(offsetof(Point, x) == 0);
static_assert(offsetof(Point, y) == 8);

constexpr std::uint_fast32_t hash(std::string_view s) noexcept
{
	std::uint_fast32_t val{};

	for (size_t i = 0; i < s.size(); ++i)
		val ^= static_cast<std::uint_fast32_t>(s[i])
		    << ((i % sizeof(std::uint_fast32_t)) * CHAR_BIT);

	return val;
}

const char *vizzu_errorMessage(const void *exceptionPtr,
    const std::type_info *typeinfo)
{
	std::string_view type_info = typeinfo->name();

	if (type_info.starts_with("St")) { // std::
		constexpr auto valid = "abcdefghijklmnopqrstuvwxyz_";
		auto last = type_info.find_last_of(valid);
		auto first = type_info.find_last_not_of(valid, last);
		type_info = type_info.substr(first + 1, last - first);

		const std::exception *realException = nullptr;

		switch (hash(type_info)) {
		case hash("exception"):
			realException =
			    static_cast<const std::exception *>(exceptionPtr);
			break;
		case hash("bad_alloc"):
			realException =
			    static_cast<const std::bad_alloc *>(exceptionPtr);
			break;
		case hash("bad_array_new_length"):
			realException =
			    static_cast<const std::bad_array_new_length *>(
			        exceptionPtr);
			break;
		case hash("bad_optional_access"):
			realException =
			    static_cast<const std::bad_optional_access *>(
			        exceptionPtr);
			break;
		case hash("logic_error"):
			realException =
			    static_cast<const std::logic_error *>(exceptionPtr);
			break;
		case hash("invalid_argument"):
			realException =
			    static_cast<const std::invalid_argument *>(
			        exceptionPtr);
			break;
		case hash("length_error"):
			realException =
			    static_cast<const std::length_error *>(exceptionPtr);
			break;
		case hash("out_of_range"):
			realException =
			    static_cast<const std::out_of_range *>(exceptionPtr);
			break;
		case hash("runtime_error"):
			realException =
			    static_cast<const std::runtime_error *>(exceptionPtr);
			break;
		case hash("bad_cast"):
			realException =
			    static_cast<const std::bad_cast *>(exceptionPtr);
			break;
		case hash("bad_any_cast"):
			realException =
			    static_cast<const std::bad_any_cast *>(exceptionPtr);
			break;
		}

		if (realException) return realException->what();
	}

	thread_local std::string data;
	data = "Cannot call '";
	data += type_info;
	data += "::what()'. Please add this exception to the ";
	data += __func__;
	data += " function.";
	return data.c_str();
}

const char *vizzu_version() { return Interface::version(); }

void vizzu_setLogging(bool enable)
{
	return Interface::setLogging(enable);
}

const void *vizzu_createChart()
{
	return Interface::getInstance().createChart();
}

void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift)
{
	return Interface::getInstance().keyPress(key, ctrl, alt, shift);
}

void vizzu_pointerMove(int pointerId, double x, double y)
{
	return Interface::getInstance().pointerMove(pointerId, x, y);
}

void vizzu_pointerDown(int pointerId, double x, double y)
{
	return Interface::getInstance().pointerDown(pointerId, x, y);
}

void vizzu_pointerUp(int pointerId, double x, double y)
{
	return Interface::getInstance().pointerUp(pointerId, x, y);
}

void vizzu_pointerLeave(int pointerId)
{
	return Interface::getInstance().pointerLeave(pointerId);
}

void vizzu_wheel(double delta)
{
	return Interface::getInstance().wheel(delta);
}

void vizzu_update(double width, double height, int renderControl)
{
	return Interface::getInstance().update(width,
	    height,
	    static_cast<Interface::RenderControl>(renderControl));
}

const char *style_getList() { return Interface::getStyleList(); }

void style_setValue(const char *path, const char *value)
{
	return Interface::getInstance().setStyleValue(path, value);
}

const char *style_getValue(const char *path, bool computed)
{
	return Interface::getInstance().getStyleValue(path, computed);
}

const void *chart_store()
{
	return Interface::getInstance().storeChart();
}

void chart_restore(const void *chart)
{
	return Interface::getInstance().restoreChart(chart);
}

const void *chart_anim_store()
{
	return Interface::getInstance().storeAnim();
}

void chart_anim_restore(const void *anim)
{
	return Interface::getInstance().restoreAnim(anim);
}

void object_free(const void *ptr)
{
	return Interface::getInstance().freeObj(ptr);
}

const char *chart_getList() { return Interface::getChartParamList(); }

const char *chart_getValue(const char *path)
{
	return Interface::getInstance().getChartValue(path);
}

void chart_setValue(const char *path, const char *value)
{
	return Interface::getInstance().setChartValue(path, value);
}

void chart_setFilter(bool (*filter)(const void *),
    void (*deleter)(bool (*)(const void *)))
{
	if (filter)
		return Interface::getInstance().setChartFilter(
		    JsFunctionWrapper<bool, const Data::RowWrapper &>{
		        {filter, deleter}});

	return Interface::getInstance().setChartFilter(
	    JsFunctionWrapper<bool, const Data::RowWrapper &>{});
}

const void *
record_getValue(void *record, const char *column, bool isDimension)
{
	return Interface::getRecordValue(record, column, isDimension);
}

void data_addDimension(const char *name,
    const char **categories,
    int count)
{
	return Interface::getInstance().addDimension(name,
	    categories,
	    count);
}

void data_addMeasure(const char *name, double *values, int count)
{
	return Interface::getInstance().addMeasure(name, values, count);
}

void data_addRecord(const char **cells, int count)
{
	return Interface::getInstance().addRecord(cells, count);
}

const char *data_metaInfo()
{
	return Interface::getInstance().dataMetaInfo();
}

void addEventListener(const char *name,
    void (*callback)(const void *event, const char *))
{
	return Interface::getInstance().addEventListener(name, callback);
}

void removeEventListener(const char *name,
    void (*callback)(const void *event, const char *))
{
	return Interface::getInstance().removeEventListener(name,
	    callback);
}

void event_preventDefault(const void *event)
{
	return Interface::getInstance().preventDefaultEvent(event);
}

void chart_animate(void (*callback)(bool))
{
	return Interface::getInstance().animate(callback);
}

const Point *chart_relToCanvasCoords(double rx, double ry)
{
	thread_local Point res;
	Interface::getInstance().relToCanvasCoords(rx, ry, res.x, res.y);
	return &res;
}

const Point *chart_canvasToRelCoords(double x, double y)
{
	thread_local Point res;
	Interface::getInstance().canvasToRelCoords(x, y, res.x, res.y);
	return &res;
}

void chart_setKeyframe()
{
	return Interface::getInstance().setKeyframe();
}

const char *chart_markerData(unsigned id)
{
	return Interface::getInstance().getMarkerData(id);
}

void anim_control(const char *command, const char *param)
{
	return Interface::getInstance().animControl(command, param);
}

void anim_setValue(const char *path, const char *value)
{
	return Interface::getInstance().setAnimValue(path, value);
}
