#include "cinterface.h"

#include "interface.h"

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

extern const char *vizzu_version()
{
	return Vizzu::Interface::getInstance().version();
}

void vizzu_setLogging(bool enable)
{
	Vizzu::Interface::setLogging(enable);
}

void vizzu_init() { Vizzu::Interface::getInstance().init(); }

void vizzu_keyPress(int key, bool ctrl, bool alt, bool shift)
{
	Vizzu::Interface::getInstance().keyPress(key, ctrl, alt, shift);
}

void vizzu_pointerMove(int pointerId, double x, double y)
{
	Vizzu::Interface::getInstance().pointerMove(pointerId, x, y);
}

void vizzu_pointerDown(int pointerId, double x, double y)
{
	Vizzu::Interface::getInstance().pointerDown(pointerId, x, y);
}

void vizzu_pointerUp(int pointerId, double x, double y)
{
	Vizzu::Interface::getInstance().pointerUp(pointerId, x, y);
}

void vizzu_pointerLeave(int pointerId)
{
	Vizzu::Interface::getInstance().pointerLeave(pointerId);
}

void vizzu_wheel(double delta)
{
	Vizzu::Interface::getInstance().wheel(delta);
}

void vizzu_poll() { Vizzu::Interface::getInstance().poll(); }

void vizzu_update(double width, double height, int renderControl)
{
	Vizzu::Interface::getInstance().update(width,
	    height,
	    static_cast<Vizzu::Interface::RenderControl>(renderControl));
}

const char *style_getList()
{
	return Vizzu::Interface::getStyleList();
}

void style_setValue(const char *path, const char *value)
{
	Vizzu::Interface::getInstance().setStyleValue(path, value);
}

const char *style_getValue(const char *path, bool computed)
{
	return Vizzu::Interface::getInstance().getStyleValue(path,
	    computed);
}

void *chart_store()
{
	return Vizzu::Interface::getInstance().storeChart();
}

void chart_restore(void *chart)
{
	Vizzu::Interface::getInstance().restoreChart(chart);
}

void *chart_anim_store()
{
	return Vizzu::Interface::getInstance().storeAnim();
}

void chart_anim_restore(void *anim)
{
	Vizzu::Interface::getInstance().restoreAnim(anim);
}

void object_free(void *ptr)
{
	Vizzu::Interface::getInstance().freeObj(ptr);
}

const char *chart_getList()
{
	return Vizzu::Interface::getChartParamList();
}

const char *chart_getValue(const char *path)
{
	return Vizzu::Interface::getInstance().getChartValue(path);
}

void chart_setValue(const char *path, const char *value)
{
	Vizzu::Interface::getInstance().setChartValue(path, value);
}

void chart_setFilter(bool (*filter)(const void *),
    void (*deleter)(bool (*)(const void *)))
{
	if (filter)
		Vizzu::Interface::getInstance().setChartFilter(
		    Vizzu::JsFunctionWrapper<bool,
		        const Vizzu::Data::RowWrapper &>{{filter, deleter}});
	else
		Vizzu::Interface::getInstance().setChartFilter(
		    Vizzu::JsFunctionWrapper<bool,
		        const Vizzu::Data::RowWrapper &>{});
}

const void *
record_getValue(void *record, const char *column, bool isDimension)
{
	return Vizzu::Interface::getRecordValue(record,
	    column,
	    isDimension);
}

void data_addDimension(const char *name,
    const char **categories,
    int count)
{
	Vizzu::Interface::getInstance().addDimension(name,
	    categories,
	    count);
}

void data_addMeasure(const char *name,
    const char *unit,
    double *values,
    int count)
{
	Vizzu::Interface::getInstance().addMeasure(name,
	    unit,
	    values,
	    count);
}

void data_addRecord(const char **cells, int count)
{
	Vizzu::Interface::getInstance().addRecord(cells, count);
}

const char *data_metaInfo()
{
	return Vizzu::Interface::getInstance().dataMetaInfo();
}

void addEventListener(const char *name,
    void (*callback)(const char *))
{
	Vizzu::Interface::getInstance().addEventListener(name, callback);
}

void removeEventListener(const char *name,
    void (*callback)(const char *))
{
	Vizzu::Interface::getInstance().removeEventListener(name,
	    callback);
}

void event_preventDefault()
{
	Vizzu::Interface::getInstance().preventDefaultEvent();
}

void chart_animate(void (*callback)(bool))
{
	Vizzu::Interface::getInstance().animate(callback);
}

const Point *chart_relToCanvasCoords(double rx, double ry)
{
	thread_local Point res;
	Vizzu::Interface::getInstance().relToCanvasCoords(rx,
	    ry,
	    res.x,
	    res.y);
	return &res;
}

const Point *chart_canvasToRelCoords(double x, double y)
{
	thread_local Point res;
	Vizzu::Interface::getInstance().canvasToRelCoords(x,
	    y,
	    res.x,
	    res.y);
	return &res;
}

void chart_setKeyframe()
{
	Vizzu::Interface::getInstance().setKeyframe();
}

const char *chart_markerData(unsigned id)
{
	return Vizzu::Interface::getInstance().getMarkerData(id);
}

void anim_control(const char *command, const char *param)
{
	Vizzu::Interface::getInstance().animControl(command, param);
}

void anim_setValue(const char *path, const char *value)
{
	Vizzu::Interface::getInstance().setAnimValue(path, value);
}
