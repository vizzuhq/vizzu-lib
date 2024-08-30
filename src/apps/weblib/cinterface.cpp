#include "cinterface.h"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <variant>

#include "interface.h"

using Vizzu::Interface;

static_assert(sizeof(double) == 8);
static_assert(offsetof(Point, x) == 0);
static_assert(offsetof(Point, y) == 8);
static_assert(offsetof(Value, dimension) == 0);
static_assert(offsetof(Value, measureValue) == 8);
static_assert(offsetof(Value, dimensionValue) == 8);

constexpr std::uint_fast32_t hash(std::string_view s) noexcept
{
	std::uint_fast32_t val{};

	for (size_t i = 0; i < s.size(); ++i)
		val ^= static_cast<std::uint_fast32_t>(s[i])
		    << ((i % sizeof(std::uint_fast32_t))
		           * std::numeric_limits<unsigned char>::digits);

	return val;
}

const char *vizzu_errorMessage(APIHandles::Exception exceptionPtr,
    const std::type_info *typeinfo)
{
	std::string_view type_info = typeinfo->name();

	if (type_info.starts_with("St")
	    || type_info.starts_with("NSt")) { // std::
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
		case hash("bad_function_call"):
			realException =
			    static_cast<const std::bad_function_call *>(
			        exceptionPtr);
			break;
		case hash("bad_variant_access"):
			realException =
			    static_cast<const std::bad_variant_access *>(
			        exceptionPtr);
			break;
		default: break;
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

void vizzu_setLogging(bool enable) { Interface::setLogging(enable); }

APIHandles::Chart vizzu_createChart()
{
	return Interface::getInstance().createChart();
}

APIHandles::Canvas vizzu_createCanvas()
{
	return Interface::getInstance().createCanvas();
}

void vizzu_pointerMove(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	Interface::getInstance().pointerMove(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerDown(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	Interface::getInstance().pointerDown(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerUp(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId,
    double x,
    double y)
{
	Interface::getInstance().pointerUp(chart,
	    canvas,
	    pointerId,
	    x,
	    y);
}

void vizzu_pointerLeave(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    int pointerId)
{
	Interface::getInstance().pointerLeave(chart, canvas, pointerId);
}

void vizzu_wheel(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double delta)
{
	Interface::getInstance().wheel(chart, canvas, delta);
}

void vizzu_update(APIHandles::Chart chart, double timeInMSecs)
{
	Interface::getInstance().update(chart, timeInMSecs);
}

void vizzu_render(APIHandles::Chart chart,
    APIHandles::Canvas canvas,
    double width,
    double height)
{
	Interface::getInstance().render(chart, canvas, width, height);
}

void vizzu_setLineResolution(APIHandles::Canvas canvas,
    double distanceMax,
    double curveHeightMax)
{
	Interface::getInstance().setLineResolution(canvas,
	    distanceMax,
	    curveHeightMax);
}

const char *style_getList() { return Interface::getStyleList(); }

void style_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	Interface::getInstance().setStyleValue(chart, path, value);
}

const char *style_getValue(APIHandles::Chart chart,
    const char *path,
    bool computed)
{
	return Interface::getInstance().getStyleValue(chart,
	    path,
	    computed);
}

APIHandles::Snapshot chart_store(APIHandles::Chart chart)
{
	return Interface::getInstance().storeChart(chart);
}

void chart_restore(APIHandles::Chart chart,
    APIHandles::Snapshot snapshot)
{
	Interface::getInstance().restoreChart(chart, snapshot);
}

APIHandles::Animation chart_anim_store(APIHandles::Chart chart)
{
	return Interface::getInstance().storeAnim(chart);
}

void chart_anim_restore(APIHandles::Chart chart,
    APIHandles::Animation anim)
{
	Interface::getInstance().restoreAnim(chart, anim);
}

void object_free(APIHandles::Any handle)
{
	Interface::getInstance().freeObj(handle);
}

const char *chart_getList() { return Interface::getChartParamList(); }

const char *chart_getValue(APIHandles::Chart chart, const char *path)
{
	return Interface::getInstance().getChartValue(chart, path);
}

void chart_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	Interface::getInstance().setChartValue(chart, path, value);
}

void chart_setFilter(APIHandles::Chart chart,
    bool (*filter)(const Vizzu::Data::RowWrapper *),
    void (*deleter)(bool (*)(const Vizzu::Data::RowWrapper *)))
{
	Interface::getInstance().setChartFilter(chart, {filter, deleter});
}

const Value *record_getValue(const Vizzu::Data::RowWrapper *record,
    const char *column)
{
	thread_local Value val{{}, {}};
	if (auto &&cval = Interface::getRecordValue(*record, column);
	    cval.index()) {
		val.dimension = true;
		auto &&dim =
		    *std::get_if<const Text::immutable_string *>(&cval);
		new (
		    &val.dimensionValue) // NOLINT(bugprone-multi-level-implicit-pointer-conversion)
		    const char *{dim ? dim->c_str() : nullptr};
	}
	else {
		val.dimension = false;
		new (&val.measureValue) double{*std::get_if<double>(&cval)};
	}
	return &val;
}

void data_addDimension(APIHandles::Chart chart,
    const char *name,
    const char **categories,
    std::uint32_t categoriesCount,
    const std::uint32_t *categoryIndices,
    std::uint32_t categoryIndicesCount)
{
	Interface::getInstance().addDimension(chart,
	    name,
	    categories,
	    categoriesCount,
	    categoryIndices,
	    categoryIndicesCount);
}

void data_addMeasure(APIHandles::Chart chart,
    const char *name,
    const char *unit,
    const double *values,
    std::uint32_t count)
{
	Interface::getInstance().addMeasure(chart,
	    name,
	    unit,
	    values,
	    count);
}

void data_addRecord(APIHandles::Chart chart,
    const char *const *cells,
    std::uint32_t count)
{
	Interface::getInstance().addRecord(chart, cells, count);
}

const char *data_metaInfo(APIHandles::Chart chart)
{
	return Interface::getInstance().dataMetaInfo(chart);
}

void addEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *))
{
	Interface::getInstance().addEventListener(chart, name, callback);
}

void removeEventListener(APIHandles::Chart chart,
    const char *name,
    void (*callback)(APIHandles::Event event, const char *))
{
	Interface::getInstance().removeEventListener(chart,
	    name,
	    callback);
}

void event_preventDefault(APIHandles::Event event)
{
	Interface::preventDefaultEvent(event);
}

void chart_animate(APIHandles::Chart chart, void (*callback)(bool))
{
	Interface::getInstance().animate(chart, callback);
}

const Point *
chart_relToCanvasCoords(APIHandles::Chart chart, double rx, double ry)
{
	thread_local Point res;
	Interface::getInstance().relToCanvasCoords(chart,
	    rx,
	    ry,
	    res.x,
	    res.y);
	return &res;
}

const Point *
chart_canvasToRelCoords(APIHandles::Chart chart, double x, double y)
{
	thread_local Point res;
	Interface::getInstance().canvasToRelCoords(chart,
	    x,
	    y,
	    res.x,
	    res.y);
	return &res;
}

void chart_setKeyframe(APIHandles::Chart chart)
{
	Interface::getInstance().setKeyframe(chart);
}

void anim_control_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	Interface::getInstance().setAnimControlValue(chart, path, value);
}

const char *anim_control_getValue(APIHandles::Chart chart,
    const char *path)
{
	return Interface::getInstance().getAnimControlValue(chart, path);
}

void anim_setValue(APIHandles::Chart chart,
    const char *path,
    const char *value)
{
	Interface::getInstance().setAnimValue(chart, path, value);
}
